#include "TreeNode.h"
#include "TreeModel.h"

#include <QCoreApplication>
#include <QStringList>
#include <QMimeData>
#include <QIODevice>
#include <QDataStream>

TreeModel::TreeModel(TreeNode * rootNode, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootNode = rootNode;
}

TreeModel::~TreeModel()
{
    delete m_rootNode;
}

TreeNode * TreeModel::rootNode() const
{
    return m_rootNode;
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
    Q_UNUSED(column); Q_UNUSED(row); Q_UNUSED(parent);
    return (mimeData->hasFormat(s_treeNodeMimeType) && action == Qt::MoveAction);
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

    if (row == -1) {
        // qDebug("Drop on node");
        if(parentNode->acceptsChildren()) {
            row = parentNode->childCount(); // insert at end of parent
        } else {
            // if node dropped onto does not accept children we intrepret
            // it as moving the nodes to that position in it's parent
            while(parentNode != 0 && !parentNode->acceptsChildren())
            {
                row = parentNode->row(); // we want to insert the new node at this row
                parentNode = parentNode->parentNode(); // find the parent of parent
            }
            Q_ASSERT(parentNode);
        }
    }else {
        // qDebug("Drop into node");
    }

    int count;
    stream >> count;
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
        removeBranche(node);

        // Insert at new position
        // qDebug() << "Inserting into" << parent << row;
        insertBranche(parentNode, row, node);
        ++row;
    }
    isUserModified = true;
    return true;
}

void TreeModel::insertBranche(TreeNode *parentNode, int row, TreeNode *node){
    const QModelIndex parent = indexForNode(parentNode);
    if(row < 0) // row < 0 means insert at end
        row = rowCount(parent);
    beginInsertRows(parent, row, row);
    parentNode->insertChild(row, node);
    endInsertRows();
}

void TreeModel::brancheGoingToBeDeleted(TreeNode *node)
{
    (void) node;// Default implementation does nothing.
}

void TreeModel::deleteBranche(TreeNode *node)
{
    brancheGoingToBeDeleted(node);
    removeBranche(node);
    delete node;
    isUserModified = true;
}

void TreeModel::deleteBranche(const QModelIndex nodeIdx)
{
    if(!nodeIdx.isValid())
        return;
    deleteBranche(nodeForIndex(nodeIdx));
}

void TreeModel::deleteBranches(const QModelIndexList indexes)
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
        deleteBranche(node);
    }
}

void TreeModel::addNode(const QModelIndex parent, TreeNode *node)
{
    TreeNode * parentNode = nodeForIndex(parent);
    int row = -1;
    // new nodes can only be added to root or Headers
    while(parentNode != 0 && !parentNode->acceptsChildren())
    {
        row = parentNode->row(); // we want to insert the new node at this row
        parentNode = parentNode->parentNode(); // find the parent of parent
    }

    if(parentNode)
    {
        insertBranche(parentNode, row, node);
        isUserModified = true;
    }
}

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;
    else
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
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

void TreeModel::removeBranche(TreeNode *node)
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
