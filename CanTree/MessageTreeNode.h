#ifndef MESSAGETREENODE_H
#define MESSAGETREENODE_H

#include <QElapsedTimer>
#include "TreeNode.h"
#include "lib-slcan/can_message.h"

#define DEBUG_TREEVIEW

class MessageTreeNode : public TreeNode
{
public:
    MessageTreeNode(const QString& name, int id, bool IDE, bool RTR);
    MessageTreeNode(const can_message_t * cmsg);
    MessageTreeNode();

    QVariant getData(dataFunction df, int role) const override;
    bool setData(dataFunction df, const QVariant &value) override;

    void update(const can_message_t * cmsg);

    void writeDataToXml(QXmlStreamWriter &writer) const override;
    void readDataFromXml(QXmlStreamReader &reader) override;

    uint32_t getId(){ return id;}
    bool getIDE(){return IDE;}
    bool getRTR(){return RTR;}

private:
    void initIdString();
    void updateDataDecoded();

    bool IDE = false;
    bool RTR = false;
    uint32_t id = 0;
    uint8_t dlc = 0;
    uint8_t data[8] = {0};

    QElapsedTimer m_timer;
    int m_count = 0;
    qint64 m_period = 0;
    double m_firstDecodedNum = 0.0;

    QString m_name;
    QString m_idString;
    QString m_dlcString;
    QString m_countString;
    QString m_periodString;
    QString m_dataString;
    QString m_dataDecodedString;
    QString m_formatString;

#ifdef DEBUG_TREEVIEW
    int m_debugCount[8] = {0};
#endif
};

#endif // MESSAGETREENODE_H
