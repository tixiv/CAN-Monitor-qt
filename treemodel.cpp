#include "headertreenode.h"
#include "treemodel.h"

#include <QCoreApplication>
#include <QStringList>
#include <QMimeData>
#include <QIODevice>
#include <QDataStream>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{

    m_rootNode = new HeaderTreeNode(0);

    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfName,        "Name"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfID,          "ID (HEX)"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfDLC,         "DLC"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfCount,       "Count"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfPeriod,      "Period (ms)"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfRawData,     "Raw Data (HEX)"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfDataDecoded, "Decoded Data"));
    m_columnFunctions.append(QPair<enum dataFunction,QVariant>(dfFormat,      "Format String"));
}

TreeModel::~TreeModel()
{
    delete m_rootNode;
}

TreeNode * TreeModel::rootNode() const
{
    return m_rootNode;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_columnFunctions.count();
}

static const char s_treeNodeMimeType[] = "application/x-treenode";

//returns the mime type
QStringList TreeModel::mimeTypes() const
{
    return QStringList() << s_treeNodeMimeType;
}

//receives a list of model indexes list
QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData;
    QByteArray data; //a kind of RAW format for datas

    //QDataStream is independant on the OS or proc architecture
    //serialization of C++'s basic data types, like char, short, int, char *, etc.
    //Serialization of more complex data is accomplished
    //by breaking up the data into primitive units.
    QDataStream stream(&data, QIODevice::WriteOnly);
    QList<TreeNode *> nodes;

    //
    foreach (const QModelIndex &index, indexes) {
        TreeNode *node = nodeForIndex(index);
        if (!nodes.contains(node))
            nodes << node;
    }
    stream << QCoreApplication::applicationPid();
    stream << nodes.count();
    foreach (TreeNode *node, nodes) {
        stream << reinterpret_cast<qlonglong>(node);
    }
    mimeData->setData(s_treeNodeMimeType, data);
    return mimeData;
}

bool TreeModel::canDropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column); Q_UNUSED(row);
    if (!mimeData->hasFormat(s_treeNodeMimeType) || action != Qt::MoveAction) {
        return false;
    }
    return nodeForIndex(parent)->dropAllowed();
}

bool TreeModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_ASSERT(action == Qt::MoveAction);
    Q_UNUSED(column);
    if (!mimeData->hasFormat(s_treeNodeMimeType)) {
        return false;
    }
    QByteArray data = mimeData->data(s_treeNodeMimeType);
    QDataStream stream(&data, QIODevice::ReadOnly);
    qint64 senderPid;
    stream >> senderPid;
    if (senderPid != QCoreApplication::applicationPid()) {
        // Let's not cast pointers that come from another process...
        return false;
    }
    TreeNode *parentNode = nodeForIndex(parent);
    Q_ASSERT(parentNode);
    int count;
    stream >> count;
    if (row == -1) {
        // valid index means: drop onto item. I chose that this should insert
        // a child item, because this is the only way to create the first child of an item...
        // This explains why Qt calls it parent: unless you just support replacing, this
        // is really the future parent of the dropped items.
        if (parent.isValid())
            row = 0;
        else
            // invalid index means: append at bottom, after last toplevel
            row = rowCount(parent);
    }
    for (int i = 0; i < count; ++i) {
        // Decode data from the QMimeData
        qlonglong nodePtr;
        stream >> nodePtr;
        TreeNode *node = reinterpret_cast<TreeNode *>(nodePtr);

        // Adjust destination row for the case of moving an item
        // within the same parent, to a position further down.
        // Its own removal will reduce the final row number by one.
        if (node->row() < row && parentNode == node->parentNode())
            --row;

        // Remove from old position
        removeNode(node);

        // Insert at new position
        //qDebug() << "Inserting into" << parent << row;
        insertNode(parentNode, row, node);
        ++row;
    }
    return true;
}

void TreeModel::insertNode(TreeNode *parentNode, int row, TreeNode *node){
    const QModelIndex parent = indexForNode(parentNode);
    if(row < 0)
        // insert at end
        row = rowCount(parent);
    beginInsertRows(parent, row, row);
    parentNode->insertChild(row, node);
    endInsertRows();
}

void TreeModel::insertNode(const QModelIndex parent, int row, TreeNode *node){
    TreeNode *parentNode = nodeForIndex(parent);
    if(row < 0)
        // insert at end
        row = rowCount(parent);
    beginInsertRows(parent, row, row);
    parentNode->insertChild(row, node);
    endInsertRows();
}

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return QVariant();

    TreeNode *node = nodeForIndex(index);

    return node->getData(m_columnFunctions.value(index.column()).first);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    TreeNode *node = nodeForIndex(index);

    return node->setData(m_columnFunctions.value(index.column()).first, value);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    Qt::ItemFlags flags = 0;
    switch(m_columnFunctions.value(index.column()).first){
    case dfName:
    case dfFormat:
        flags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable ; break;
    }

    return flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_columnFunctions.value(section).second;
    }

    return QVariant();
}

//returns a pointer to the "index"
TreeNode * TreeModel::nodeForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootNode;
    else
        return static_cast<TreeNode*>(index.internalPointer());
}

QModelIndex TreeModel::indexForNode(TreeNode * node, int column) const
{
    if(node == m_rootNode)
        return QModelIndex();
    const int row = node->row();
    return createIndex(row, column, node);
}

void TreeModel::removeNode(TreeNode *node)
{
    const int row = node->row();
    QModelIndex idx = createIndex(row, 0, node);
    beginRemoveRows(idx.parent(), row, row);
    node->parentNode()->removeChild(row);
    endRemoveRows();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentNode = nodeForIndex(parent);

    TreeNode *childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    TreeNode *childNode = nodeForIndex(index);
    if (childNode == m_rootNode)
        return QModelIndex();

    TreeNode *parentNode = childNode->parentNode();
    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    TreeNode *parentNode = nodeForIndex(parent);
    return parentNode->childCount();
}
