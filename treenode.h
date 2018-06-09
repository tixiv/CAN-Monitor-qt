#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include <QVariant>

enum dataFunction{
    dfName,
    dfID,
    dfDLC,
    dfCount,
    dfRawData,
    dfDataDecoded,
    dfFormat,
};

class TreeNode
{
public:
    explicit TreeNode(TreeNode *parentNode, const QVariant name="");
    ~TreeNode();

    void appendChild(TreeNode *child);
    void removeChild(int row);

    TreeNode *child(int row) const;
    int childCount() const;
    int row() const;
    TreeNode *parentNode() const;
    void insertChild(int pos, TreeNode *child);



    virtual QVariant getData(dataFunction df) const;

private:
    QList<TreeNode*> m_childNodes;
    TreeNode *m_parentNode;

    QVariant m_name;
};

#endif // TREENODE_H
