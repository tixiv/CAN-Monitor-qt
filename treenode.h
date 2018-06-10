#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include <QVariant>

enum dataFunction{
    dfName,
    dfID,
    dfDLC,
    dfCount,
    dfPeriod,
    dfRawData,
    dfDataDecoded,
    dfFormat,
};

class TreeNode
{
public:
    explicit TreeNode(const QVariant name="");
    ~TreeNode();

    void appendChild(TreeNode *child);
    void removeChild(int row);

    TreeNode *child(int row) const;
    int childCount() const;
    int row() const;
    TreeNode *parentNode() const;
    void insertChild(int pos, TreeNode *child);


    virtual bool dropAllowed() const;
    virtual QVariant getData(dataFunction df) const;
    virtual bool setData(dataFunction df, const QVariant &value);

protected:
    QVariant m_name;
private:
    QList<TreeNode*> m_childNodes;
    TreeNode *m_parentNode;
};

#endif // TREENODE_H
