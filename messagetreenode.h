#ifndef MESSAGETREENODE_H
#define MESSAGETREENODE_H

#include "treenode.h"

class MessageTreeNode : public TreeNode
{
public:
    MessageTreeNode(TreeNode *parent, const QVariant name="");
};

#endif // MESSAGETREENODE_H
