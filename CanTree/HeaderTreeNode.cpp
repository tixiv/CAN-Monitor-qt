#include "HeaderTreeNode.h"

HeaderTreeNode::HeaderTreeNode(const QVariant &name)
    : TreeNode(), m_name(name)
{
}

bool HeaderTreeNode::dropAllowed() const
{
    return true;
}

QVariant HeaderTreeNode::getData(dataFunction df) const
{
    if(df == dfName){
        return m_name;
    }else{
        return QVariant();
    }
}

bool HeaderTreeNode::setData(dataFunction df, const QVariant &value)
{
    if(df == dfName){
        m_name = value;
        return true;
    }else{
        return false;
    }
}
