#include "CanTreeModel.h"



CanTreeModel::CanTreeModel()
    : TreeModel()
{

}

void CanTreeModel::inputMessage(const can_message_t * cmsg){
    uint32_t uid = CanUniqueID(cmsg).val;

    if(map.contains(uid)){
        MessageTreeNode * node = map[uid];
        node->update(cmsg);

        QModelIndex miLeft = indexForNode(node, 2);
        QModelIndex miRight = indexForNode(node, columnCount()-1);
        emit dataChanged(miLeft, miRight);
    }
    else
    {
        MessageTreeNode * node = new MessageTreeNode(cmsg);
        map[uid] = node;
        insertNode(QModelIndex(), -1, node);
    }
}

CanUniqueID::CanUniqueID(const can_message_t * cmsg)
{
    val = cmsg->id & 0x1fffffff;
    val |= cmsg->IDE << 31;
    val |= cmsg->RTR << 30;
}
