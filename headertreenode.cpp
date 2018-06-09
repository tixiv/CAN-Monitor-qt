#include "headertreenode.h"

HeaderTreeNode::HeaderTreeNode(TreeNode *parent, const QVariant name)
    :TreeNode(parent, name)
{
}

bool HeaderTreeNode::dropAllowed() const
{
    return true;
}
