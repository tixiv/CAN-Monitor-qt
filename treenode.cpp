#include "TreeNode.h"

#include <QStringList>

TreeNode::TreeNode(const QList<QVariant> &data, TreeNode *parent)
    : m_nodeData(data), m_parentNode(parent)
{
}

TreeNode::~TreeNode()
{
    qDeleteAll(m_childNodes);
}

void TreeNode::appendChild(TreeNode *node)
{
    m_childNodes.append(node);
}

void TreeNode::removeChild(int row)
{
    m_childNodes.removeAt(row);
}

TreeNode *TreeNode::child(int row) const
{
    return m_childNodes.value(row);
}

int TreeNode::childCount() const
{
    return m_childNodes.count();
}

int TreeNode::columnCount() const
{
    return m_nodeData.count();
}

QVariant TreeNode::data(int column) const
{
    return m_nodeData.value(column);
}

TreeNode *TreeNode::parentNode() const
{
    return m_parentNode;
}

int TreeNode::row() const
{
    if (m_parentNode)
        return m_parentNode->m_childNodes.indexOf(const_cast<TreeNode*>(this));

    return 0;
}

void TreeNode::insertChild(int pos, TreeNode *child)
{
    m_childNodes.insert(pos, child);
    child->m_parentNode = this;
}
