#include "CanTableNode.h"
#include "util/canMessageUtil.h"
#include <QTextStream>
#include <QTime>

CanTableNode::CanTableNode(can_message_t * cmsg)
{
    if(cmsg)
    {
        m_timeStampString = QTime::currentTime().toString("hh:mm:ss.zzz");
        m_idString = generateIdString(cmsg);
        m_dlcString = QString().number(cmsg->dlc);
        m_dataString = generateDataString(cmsg);
    }
}

QVariant CanTableNode::getData(CanTableColumnFunction cf, int role) const
{
    (void) role;
    switch(cf)
    {
    case ctf_timestamp: return m_timeStampString;
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
    (void) writer;
}

void CanTableNode::readDataFromXml(QXmlStreamReader &reader)
{
    (void) reader;
}

void CanTableNode::writeToStream(QTextStream &out)
{
    out << m_timeStampString.toString() << "; " << m_idString.toString() << "; " << m_dataString.toString() << "\n";
}
