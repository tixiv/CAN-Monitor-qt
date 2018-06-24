#ifndef TREENODE_H
#define TREENODE_H

#include <QVector>
#include <QVariant>


class TreeNode
{
public:
    TreeNode();
    virtual ~TreeNode();

    void appendChild(TreeNode *child);
    void removeChild(int row);

    TreeNode *child(int row) const;
    int childCount() const;
    int row() const;
    TreeNode *parentNode() const;
    void insertChild(int pos, TreeNode *child);

    virtual bool dropAllowed() const;
private:
    QVector<TreeNode*> m_childNodes;
    TreeNode *m_parentNode;
};

#endif // TREENODE_H
