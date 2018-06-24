#include "ParameterTreeModel.h"

#include "ParameterGroupNode.h"

ParameterTreeModel::ParameterTreeModel()
    : TreeModel(new ParameterGroupNode())
{
    m_columnFunctions = {
        ColumnRole(pcf_name,        "Name"),
        ColumnRole(pcf_command,     "Command"),
        ColumnRole(pcf_subCommand,  "Subcommand"),
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
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::UserRole))
        return QVariant();

    ParameterTreeNode *node = static_cast<ParameterTreeNode*>(nodeForIndex(index));

    return node->getData(m_columnFunctions.at(index.column()).df, role);
}

bool ParameterTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    isUserModified = true;

    ParameterTreeNode *node = static_cast<ParameterTreeNode*>(nodeForIndex(index));
    if(node->setData(m_columnFunctions.at(index.column()).df, value))
    {
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

    Qt::ItemFlags flags = 0;
    if(m_columnFunctions.at(index.column()).df != pcf_value){
        flags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable ;
    }

    return flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
