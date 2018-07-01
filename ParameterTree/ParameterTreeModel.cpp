#include "ParameterTreeModel.h"
#include "ParameterGroupNode.h"
#include "ParameterNode.h"
#include "ParameterTreeNodeFactory.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

ParameterTreeModel::ParameterTreeModel()
    : TreeModel(new ParameterGroupNode())
{
    m_columnFunctions = {
        ColumnRole(pcf_name,        "Name"),
        ColumnRole(pcf_command,     "Command"),
        ColumnRole(pcf_subCommand,  "Subcommand"),
        ColumnRole(pcf_access,      "Access"),
        ColumnRole(pcf_value,       "Value"),
        ColumnRole(pcf_newValue,    "New Value"),
        ColumnRole(pcf_unit,        "Unit"),
    };
}

int ParameterTreeModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_columnFunctions.count();
}

QVariant ParameterTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_columnFunctions.at(section).name;
    }

    return QVariant();
}

QVariant ParameterTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::UserRole && role != Qt::BackgroundRole))
        return QVariant();

    ParameterTreeNode *node = static_cast<ParameterTreeNode*>(nodeForIndex(index));

    return node->getData(m_columnFunctions.at(index.column()).df, role);
}

bool ParameterTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    ParameterTreeNode *node = static_cast<ParameterTreeNode*>(nodeForIndex(index));
    auto pcf = m_columnFunctions.at(index.column()).df;
    if(node->setData(pcf, value))
    {
        if(pcf == pcf_newValue)
            emit newValueEdited(node);
        else
            isUserModified = true;

        emit(dataChanged(index, index));
        return true;
    }
    else
        return false;
}

Qt::ItemFlags ParameterTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    // get is editable flag
    Qt::ItemFlags flags = static_cast<ParameterTreeNode*>(nodeForIndex(index))->getFlags(m_columnFunctions.at(index.column()).df, m_editModeActive);

    if(m_editModeActive)
        flags |= Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsSelectable;

    return flags | Qt::ItemIsEnabled;
}

void ParameterTreeModel::writeTreeToXml(QXmlStreamWriter &writer)
{
    ParameterTreeNodeFactory factory;
    writer.writeStartElement("ParameterTree");
    static_cast<XmlTreeNode*>(rootNode())->writeBranchToXml(writer, factory);
    writer.writeEndElement();
}

bool ParameterTreeModel::readTreeFromXml(QXmlStreamReader &reader)
{

    if (!reader.readNextStartElement())
        return false;
    if (reader.name() != "ParameterTree")
        return false;

    ParameterTreeNodeFactory factory;
    XmlTreeNode * loadRoot = XmlTreeNode::readBranchFromXml(reader, factory);
    reader.readNextStartElement(); // exit "ParameterTree"

    for(int i=0; i < loadRoot->childCount(); i++)
    {
        insertBranche(rootNode(), -1, loadRoot->child(i));
    }

    return true;
}

void ParameterTreeModel::inputMessage(uint8_t command, uint8_t subCommand, int32_t value)
{
    QList<TreeNode*> changed;
    rootNode()->for_tree([=,&changed](TreeNode *node){
        if(auto pn = dynamic_cast<ParameterNode*>(node))
        {
            if(pn->processMessage(command, subCommand, value))
                changed.append(node);
        }
    });
    foreach (auto node, changed) {
        emitDataChanged(node, 0, columnCount()-1);
    }
}

void ParameterTreeModel::setEditMode(bool active)
{
    m_editModeActive = active;
}
