#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include <QVariant>

class TreeNode
{
public:
    explicit TreeNode(const QList<QVariant> &data, TreeNode *parentNode);
    ~TreeNode();

    void appendChild(TreeNode *child);
    void removeChild(int row);

    TreeNode *child(int row) const;
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeNode *parentNode() const;
    void insertChild(int pos, TreeNode *child);

private:
    QList<TreeNode*> m_childNodes;
    QList<QVariant> m_nodeData;
    TreeNode *m_parentNode;
};

#endif // TREENODE_H
