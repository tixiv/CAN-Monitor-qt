#include "treenode.h"

#include <QStringList>

TreeNode::TreeNode(const QVariant name)
    : m_name(name), m_parentNode(0)
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

bool TreeNode::dropAllowed() const
{
    return false;
}

QVariant TreeNode::getData(dataFunction df) const
{
    if(df == dfName){
        return m_name;
    }else{
        return QVariant();
    }
}

bool TreeNode::setData(dataFunction df, const QVariant &value)
{
    if(df == dfName){
        m_name = value;
        return true;
    }else{
        return false;
    }
}
