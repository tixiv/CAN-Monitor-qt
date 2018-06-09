#include "TreeNode.h"

#include <QStringList>

TreeNode::TreeNode(TreeNode *parent, const QVariant name)
    : m_name(name), m_parentNode(parent)
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


QVariant TreeNode::getData(dataFunction df) const
{
    if(df == dfName){
        return m_name;
    }else{
        return QVariant();
    }
}
