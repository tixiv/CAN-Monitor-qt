#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeNode;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(TreeNode * rootNode, QObject *parent=0);
    ~TreeModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent) Q_DECL_OVERRIDE;


    // is called before a branch of nodes is going to be removed
    // from the model and deleted.
    // the user can do actions on it that need to be done before.
    virtual void brancheGoingToBeDeleted(TreeNode * root);

    void deleteBranche(TreeNode *root);
    void deleteBranche(const QModelIndex nodeIdx);
    void deleteBranches(const QModelIndexList indexes);

    void addNode(const QModelIndex parent, TreeNode *node);

    TreeNode * rootNode() const;

    bool isUserModified = false;
protected:
    void insertBranche(TreeNode *parentNode, int row, TreeNode *node);
    void removeBranche(TreeNode *node);

    QModelIndex indexForNode(TreeNode * node, int coumn=0) const;
    TreeNode * nodeForIndex(const QModelIndex &index) const;

private:
    TreeNode *m_rootNode;
};

#endif // TREEMODEL_H
