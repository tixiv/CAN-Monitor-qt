#include "CanTreeModel.h"

#include "HeaderTreeNode.h"
#include "MessageTreeNode.h"


CanTreeModel::CanTreeModel()
    : TreeModel(new HeaderTreeNode())
{
    m_columnFunctions = {
        ColumnRole(dfName,        "Name"),
        ColumnRole(dfID,          "ID (HEX)"),
        ColumnRole(dfDLC,         "DLC"),
        ColumnRole(dfCount,       "Count"),
        ColumnRole(dfPeriod,      "Period (ms)"),
        ColumnRole(dfRawData,     "Raw Data (HEX)"),
        ColumnRole(dfDataDecoded, "Decoded Data"),
        ColumnRole(dfFormat,      "Format String"),
    };
    m_columnCount = m_columnFunctions.count();
}

int CanTreeModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_columnCount;
}

QVariant CanTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_columnFunctions.at(section).name;
    }

    return QVariant();
}

QVariant CanTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::UserRole))
        return QVariant();

    TreeNode *node = nodeForIndex(index);

    return node->getData(m_columnFunctions.at(index.column()).df, role);
}

bool CanTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    isUserModified = true;

    TreeNode *node = nodeForIndex(index);
    if(node->setData(m_columnFunctions.at(index.column()).df, value))
    {
        emit(dataChanged(index, index));
        return true;
    }
    else
        return false;
}

Qt::ItemFlags CanTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    Qt::ItemFlags flags = 0;
    switch(m_columnFunctions.at(index.column()).df){
    case dfName:
    case dfFormat:
        flags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable ; break;
    default: break;
    }

    return flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void CanTreeModel::deleteNode(TreeNode *node)
{
    removeNode(node);
    unlinkNodes(node);
    delete node;
    isUserModified = true;
}

void CanTreeModel::deleteNode(const QModelIndex nodeIdx)
{
    if(!nodeIdx.isValid())
        return;
    TreeNode *node = nodeForIndex(nodeIdx);
    deleteNode(node);
}

void CanTreeModel::deleteNodes(const QModelIndexList indexes)
{
    QList<TreeNode*> nodesToDelete;
    foreach (auto index, indexes) {
        if(index.isValid()){
            auto node = nodeForIndex(index);
            if(!nodesToDelete.contains(node))
                nodesToDelete.append(node);
        }
    }

    foreach (auto node, nodesToDelete) {
        deleteNode(node);
    }
}

void CanTreeModel::addNode(const QModelIndex parent, TreeNode *node)
{
    TreeNode * parentNode = nodeForIndex(parent);
    int row = -1;
    // new nodes can only be added to root or Headers
    while(parentNode != rootNode() && !dynamic_cast<const HeaderTreeNode *>(parentNode))
    {
        row = parentNode->row(); // we want to insert the new node at this row
        parentNode = parentNode->parentNode(); // find the parent of parent
    }
    insertNode(parentNode, row, node, true);
}

void CanTreeModel::emitDataChanged(TreeNode * node, int columnLeft, int columnRight)
{
    bool emitAllAtOnce = false;
    if(emitAllAtOnce)
    {
        // this would be the official Qt way to do this,
        // but sadly Qt's implementation redraws the complete
        // View at the moment (QT 5.11.1) if more than one cell is
        // changed at once.
        QModelIndex miLeft = indexForNode(node, columnLeft);
        QModelIndex miRight = indexForNode(node, columnRight);
        emit dataChanged(miLeft, miRight);
    }
    else for(int i=columnLeft; i<=columnRight; i++)
    {
        // emiting the signals one cell at a time actually gives a
        // nice performance bonus as not the complete view needs
        // to be redrawn when one message changes.
        QModelIndex idx = indexForNode(node, i);
        emit dataChanged(idx, idx);
    }
}

void CanTreeModel::inputMessage(const can_message_t * cmsg){
    uint32_t uid = CanUniqueID(cmsg).val;

    if(map.contains(uid)){
        MessageTreeNode * node = map[uid];
        node->update(cmsg);
        emitDataChanged(node, 2, columnCount()-2);
    }
    else
    {
        MessageTreeNode * node = new MessageTreeNode(cmsg);
        map[uid] = node;
        insertNode(QModelIndex(), -1, node);
    }
}

bool CanTreeModel::linkMessageNode(MessageTreeNode * node)
{
    uint32_t uid = CanUniqueID(node->getId(), node->getIDE(), node->getRTR()).val;

    if(map.contains(uid))
        return false;

    map[uid] = node;
    return true;
}

void CanTreeModel::unlinkNodes(TreeNode * node)
{
    MessageTreeNode *mtn = dynamic_cast<MessageTreeNode *>(node);

    if(mtn){
        uint32_t uid = CanUniqueID(mtn->getId(), mtn->getIDE(), mtn->getRTR()).val;
        if(map.contains(uid))
            map.remove(uid);
    }

    for(int i=0; i<node->childCount(); i++)
    {
        unlinkNodes(node->child(i));
    }
}

static void writeNodeToXml(QXmlStreamWriter &writer, const TreeNode * node){
    if(dynamic_cast<const HeaderTreeNode *>(node)){
        writer.writeStartElement("HeaderTreeNode");
    }else if(dynamic_cast<const MessageTreeNode *>(node)){
        writer.writeStartElement("MessageTreeNode");
    }else
        return;

    node->writeDataToXml(writer);
    for(int i = 0; i < node->childCount(); i++){
        writeNodeToXml(writer, node->child(i));
    }
    writer.writeEndElement();
}

void CanTreeModel::readXmlToNode(TreeNode * parent, QXmlStreamReader &reader)
{
    while(reader.readNextStartElement())
    {
        if(reader.name() == "HeaderTreeNode"){
            HeaderTreeNode * htn = new HeaderTreeNode();
            htn->readDataFromXml(reader);
            insertNode(parent, -1, htn);
            readXmlToNode(htn, reader);
            reader.readElementText();
        }else if(reader.name() == "MessageTreeNode"){
            MessageTreeNode * mtn = new MessageTreeNode();
            mtn->readDataFromXml(reader);
            if(linkMessageNode(mtn))
                insertNode(parent, -1, mtn);
            else
                delete mtn;
            reader.readElementText();
        }else
            reader.skipCurrentElement();
    }
}

void CanTreeModel::writeTreeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartDocument();
    writer.writeStartElement("CanTree");
    writeNodeToXml(writer, rootNode());
    writer.writeEndElement();
    writer.writeEndDocument();
}

bool CanTreeModel::readTreeFromXml(QXmlStreamReader &reader)
{
    if (!reader.readNextStartElement())
        return false;
    if (reader.name() != "CanTree")
        return false;
    if (!reader.readNextStartElement())
        return false;
    if (reader.name() != "HeaderTreeNode") // the root node
        return false;

    readXmlToNode(rootNode(), reader);

    return true;
}



CanUniqueID::CanUniqueID(const can_message_t * cmsg)
{
    val = cmsg->id & 0x1fffffff;
    val |= cmsg->IDE << 31;
    val |= cmsg->RTR << 30;
}

CanUniqueID::CanUniqueID(uint32_t id, bool IDE, bool RTR)
{
    val = id & 0x1fffffff;
    val |= (IDE?1:0) << 31;
    val |= (RTR?1:0) << 30;
}
