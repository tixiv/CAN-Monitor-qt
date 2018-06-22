#ifndef TREENODE_H
#define TREENODE_H

#include <QVector>
#include <QVariant>

class QXmlStreamWriter;
class QXmlStreamReader;

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
    virtual QVariant getData(dataFunction df, int role) const = 0;
    virtual bool setData(dataFunction df, const QVariant &value) = 0;

    virtual void writeDataToXml(QXmlStreamWriter &writer) const = 0;
    virtual void readDataFromXml(QXmlStreamReader &reader) = 0;

private:
    QVector<TreeNode*> m_childNodes;
    TreeNode *m_parentNode;
};

#endif // TREENODE_H
