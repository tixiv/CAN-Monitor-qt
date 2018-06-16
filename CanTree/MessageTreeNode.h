#ifndef MESSAGETREENODE_H
#define MESSAGETREENODE_H

#include <QElapsedTimer>
#include "TreeNode.h"
#include "lib-slcan/can_message.h"

class MessageTreeNode : public TreeNode
{
public:
    MessageTreeNode(const QVariant name, int id, bool IDE, bool RTR);
    MessageTreeNode(const can_message_t * cmsg);

    QVariant getData(dataFunction df) const override;
    bool setData(dataFunction df, const QVariant &value) override;

    void update(const can_message_t * cmsg);

private:
    bool IDE = false;
    bool RTR = false;
    uint32_t id = 0;
    uint8_t dlc = 0;
    uint8_t data[8] = {0};

    QElapsedTimer m_timer;
    int m_count = 0;

    QString m_idString;
    QString m_dlcString;
    QString m_dataString;
    QString m_countString;
    QString m_periodString;
};

#endif // MESSAGETREENODE_H
