#ifndef CANTREEMODEL_H
#define CANTREEMODEL_H

#include "treemodel.h"
#include <QHash>
#include "can_message.h"
#include "messagetreenode.h"

struct CanUniqueID
{
    CanUniqueID(const can_message_t * cmsg);
    //int IDE:1;
    //int RTR:1;
    //int id:29;
    uint32_t val;
};

class CanTreeModel : public TreeModel
{
public:
    CanTreeModel();
    void inputMessage(const can_message_t * cmsg);

private:
    TreeModel * treeModel;

    QHash<uint32_t, MessageTreeNode *> map;

};

#endif // CANTREEMODEL_H