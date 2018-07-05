#ifndef CANTABLENODE_H
#define CANTABLENODE_H

#include "Tree/XmlTreeNode.h"
#include "lib-slcan/can_message.h"

class QTextStream;

enum CanTableColumnFunction
{
    ctf_timestamp,
    ctf_id,
    ctf_dlc,
    ctf_data,
};

class CanTableNode : public XmlTreeNode
{
public:
    CanTableNode(can_message_t * cmsg = 0);

    virtual QVariant getData(CanTableColumnFunction cf, int role) const;
    virtual void writeDataToXml(QXmlStreamWriter &writer) const;
    virtual void readDataFromXml(QXmlStreamReader &reader);
    bool acceptsChildren() const override;
    void writeToStream(QTextStream &out);
private:
    QVariant m_timeStampString;
    QVariant m_idString;
    QVariant m_dlcString;
    QVariant m_dataString;
};

#endif // CANTABLENODE_H
