#include "TreeNode.h"

#include <QStringList>

TreeNode::TreeNode()
    : m_parentNode(0)
{
}

TreeNode::~TreeNode()
{
    qDeleteAll(m_childNodes);
}

void TreeNode::removeChild(int row)
{
    m_childNodes.removeAt(row);
}

TreeNode *TreeNode::child(int row) const
{
    return m_childNodes.at(row);
}

int TreeNode::childCount() const
{
    return m_childNodes.count();
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

void TreeNode::appendChild(TreeNode *child)
{
    m_childNodes.append(child);
    child->m_parentNode = this;
}

void TreeNode::insertChild(int pos, TreeNode *child)
{
    m_childNodes.insert(pos, child);
    child->m_parentNode = this;
}

bool TreeNode::acceptsChildren() const
{
    return false;
}

void TreeNode::for_tree(std::function<void (TreeNode *)> f)
{
    f(this);
    foreach (auto node, m_childNodes) {
        node->for_tree(f);
    }
}
