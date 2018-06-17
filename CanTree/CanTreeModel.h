#ifndef CANTREEMODEL_H
#define CANTREEMODEL_H

#include "TreeModel.h"
#include <QHash>
#include "lib-slcan/can_message.h"
#include "MessageTreeNode.h"

struct CanUniqueID
{
    CanUniqueID(const can_message_t * cmsg);
    uint32_t val;
};

class CanTreeModel : public TreeModel
{
public:
    CanTreeModel();
    void inputMessage(const can_message_t * cmsg);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QList<QPair<enum dataFunction,QVariant>> m_columnFunctions;


    QHash<uint32_t, MessageTreeNode *> map;

};

#endif // CANTREEMODEL_H
