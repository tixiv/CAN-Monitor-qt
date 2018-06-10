#ifndef HEADERTREENODE_H
#define HEADERTREENODE_H

#include "treenode.h"

class HeaderTreeNode : public TreeNode
{
public:
    HeaderTreeNode(const QVariant name="");
    bool dropAllowed() const override;
};

#endif // HEADERTREENODE_H
