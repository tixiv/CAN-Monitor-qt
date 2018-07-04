#include "CanTableNode.h"
#include "util/canMessageUtil.h"

CanTableNode::CanTableNode(can_message_t * cmsg)
{
    if(cmsg)
    {
        m_idString = generateIdString(cmsg);
        m_dlcString = QString().number(cmsg->dlc);
        m_dataString = generateDataString(cmsg);
    }
}

QVariant CanTableNode::getData(CanTableColumnFunction cf, int role) const
{
    switch(cf)
    {
    case ctf_timestamp: return "";
    case ctf_id: return m_idString;
    case ctf_dlc: return m_dlcString;
    case ctf_data: return m_dataString;
    default: return QVariant();
    }
}

bool CanTableNode::acceptsChildren() const
{
    return true;
}

void CanTableNode::writeDataToXml(QXmlStreamWriter &writer) const
{

}

void CanTableNode::readDataFromXml(QXmlStreamReader &reader)
{

}
