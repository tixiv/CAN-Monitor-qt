#ifndef HEADERTREENODE_H
#define HEADERTREENODE_H

#include "TreeNode.h"

class HeaderTreeNode : public TreeNode
{
public:
    HeaderTreeNode(const QVariant &name="");
    bool dropAllowed() const override;

    QVariant getData(dataFunction df) const override;
    bool setData(dataFunction df, const QVariant &value) override;

private:
    QVariant m_name;
};

#endif // HEADERTREENODE_H
