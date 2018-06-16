#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "TreeNode.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void insertNode(TreeNode *parentNode, int row, TreeNode *node);
    void insertNode(const QModelIndex parent, int row, TreeNode *node);

    TreeNode * rootNode() const;
protected:
    QModelIndex indexForNode(TreeNode * node, int coumn=0) const;
    TreeNode * nodeForIndex(const QModelIndex &index) const;

private:
    void removeNode(TreeNode *node);
    void setupModelData(const QStringList &lines, TreeNode *parent);

    TreeNode *m_rootNode;

    QList<QPair<enum dataFunction,QVariant>> m_columnFunctions;
};

#endif // TREEMODEL_H