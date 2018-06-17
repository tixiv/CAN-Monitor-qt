#include "CanTreeModel.h"

#include "HeaderTreeNode.h"
#include "MessageTreeNode.h"


CanTreeModel::CanTreeModel()
    : TreeModel()
{
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfName,        "Name"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfID,          "ID (HEX)"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfDLC,         "DLC"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfCount,       "Count"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfPeriod,      "Period (ms)"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfRawData,     "Raw Data (HEX)"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfDataDecoded, "Decoded Data"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfFormat,      "Format String"));
}

int CanTreeModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_columnFunctions.count();
}

QVariant CanTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_columnFunctions.value(section).second;
    }

    return QVariant();
}

QVariant CanTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return QVariant();

    TreeNode *node = nodeForIndex(index);

    return node->getData(m_columnFunctions.value(index.column()).first);
}

bool CanTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    TreeNode *node = nodeForIndex(index);

    return node->setData(m_columnFunctions.value(index.column()).first, value);
}

Qt::ItemFlags CanTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    Qt::ItemFlags flags = 0;
    switch(m_columnFunctions.value(index.column()).first){
    case dfName:
    case dfFormat:
        flags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable ; break;
    default: break;
    }

    return flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


void CanTreeModel::inputMessage(const can_message_t * cmsg){
    uint32_t uid = CanUniqueID(cmsg).val;

    if(map.contains(uid)){
        MessageTreeNode * node = map[uid];
        node->update(cmsg);

        QModelIndex miLeft = indexForNode(node, 2);
        QModelIndex miRight = indexForNode(node, columnCount()-1);
        emit dataChanged(miLeft, miRight);
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
