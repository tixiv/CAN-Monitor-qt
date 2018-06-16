#include "HeaderTreeNode.h"

HeaderTreeNode::HeaderTreeNode(const QVariant name)
    :TreeNode(name)
{
}

bool HeaderTreeNode::dropAllowed() const
{
    return true;
}
