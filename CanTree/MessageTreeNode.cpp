#include "MessageTreeNode.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

MessageTreeNode::MessageTreeNode()
    :TreeNode(), IDE(false), RTR(false), id(0)
{
}

MessageTreeNode::MessageTreeNode(const QVariant &name, int id, bool IDE, bool RTR)
    :TreeNode(), m_name(name), IDE(IDE), RTR(RTR), id(id)
{
}

MessageTreeNode::MessageTreeNode(const can_message_t * cmsg)
    :IDE(cmsg->IDE), RTR(cmsg->RTR), id(cmsg->id)
{
    initIdString();

    m_name = "ID " + m_idString;

    update(cmsg);
}

void MessageTreeNode::initIdString()
{
    if(IDE) m_idString += "E";
    if(RTR) m_idString += "R";
    if(IDE || RTR) m_idString += " ";

    if(IDE)
        m_idString += QString().sprintf("%08X", id);
    else
        m_idString += QString().sprintf("%03X", id);
}


void MessageTreeNode::update(const can_message_t * cmsg)
{
    if(m_timer.isValid()){
        qint64 elapsed = m_timer.restart();
        m_periodString = QString::number(elapsed);
    }else{
        m_timer.start();
    }
    dlc = cmsg->dlc;
    memcpy(data, cmsg->data, 8);

    m_dlcString = QString::number(dlc);
    m_dataString = "";

    if(!RTR) for(int i=0; i<dlc; i++){
        m_dataString += QString().sprintf("%02X ", data[i]);
    }

    m_count++;
    m_countString = QString::number(m_count);
}

QVariant MessageTreeNode::getData(dataFunction df) const
{
    switch(df){
    case dfName:        return m_name;
    case dfID:          return m_idString;
    case dfDLC:         return m_dlcString;
    case dfCount:       return m_countString;
    case dfPeriod:      return m_periodString;
    case dfRawData:     return m_dataString;
    case dfDataDecoded: return m_dataDecodedString;
    case dfFormat:      return m_formatString;
    }

    return QVariant();
}

bool MessageTreeNode::setData(dataFunction df, const QVariant &value)
{
    if(df == dfName){
        m_name = value;
        return true;
    }else if(df == dfFormat){
        m_formatString = value.toString();
        return true;
    }else{
        return false;
    }
}

void MessageTreeNode::writeDataToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("name", m_name.toString());
    writer.writeAttribute("id", QString().sprintf("%X", id));
    writer.writeAttribute("IDE", IDE ? "true":"false");
    writer.writeAttribute("RTR", RTR ? "true":"false");
    writer.writeAttribute("format", m_formatString);

}

void MessageTreeNode::readDataFromXml(QXmlStreamReader &reader)
{
    m_name = reader.attributes().value("name").toString();
    id = reader.attributes().value("id").toString().toUInt(0,16);
    IDE = reader.attributes().value("IDE").toString() == "true";
    RTR = reader.attributes().value("RTR").toString() == "true";
    m_formatString = reader.attributes().value("format").toString();
    initIdString();
}
