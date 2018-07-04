#include "MessageTreeNode.h"
#include "Format/format.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "util/canMessageUtil.h"

MessageTreeNode::MessageTreeNode()
    :CanTreeNode(), IDE(false), RTR(false), id(0)
{
}

MessageTreeNode::MessageTreeNode(const QString &name, int id, bool IDE, bool RTR)
    :CanTreeNode(), IDE(IDE), RTR(RTR), id(id), m_name(name)
{
}

MessageTreeNode::MessageTreeNode(const can_message_t * cmsg)
    :IDE(cmsg->IDE), RTR(cmsg->RTR), id(cmsg->id)
{
    m_idString = generateIdString(cmsg);

    m_name = "ID " + m_idString;

    update(cmsg);
}

void MessageTreeNode::updateDataDecoded()
{
    m_firstDecodedNum = 0.0;
    m_dataDecodedString = format(m_formatString, data, &m_firstDecodedNum);
}

void MessageTreeNode::update(const can_message_t * cmsg)
{
    if(m_timer.isValid()){
        m_period = m_timer.restart();
        m_periodString = QString::number(m_period);
    }else{
        m_timer.start();
    }
    dlc = cmsg->dlc;
    memcpy(data, cmsg->data, 8);

    m_dlcString = QString::number(dlc);

    m_dataString = generateDataString(cmsg);

    m_count++;
    m_countString = QString::number(m_count);
    updateDataDecoded();
}

#ifndef DEBUG_TREEVIEW
QVariant MessageTreeNode::getData(dataFunction df, int role) const
{
    if(role == Qt::UserRole){ // sort role
        switch(df){
        case dfID:          return (IDE << 30) | (RTR << 29) | id;
        case dfCount:       return m_count;
        case dfPeriod:      return m_period;
        case dfDataDecoded: return m_firstDecodedNum;
        default: break;
        }
    }

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
#else
QVariant MessageTreeNode::getData(dataFunction df, int role) const
{
    int dfi = (int)df;

    const_cast<int*>(m_debugCount)[dfi]++;

    return m_debugCount[dfi];
}
#endif

bool MessageTreeNode::setData(dataFunction df, const QVariant &value)
{
    if(df == dfName){
        m_name = value.toString();
        return true;
    }else if(df == dfFormat){
        m_formatString = value.toString();
        updateDataDecoded();
        return true;
    }else{
        return false;
    }
}

void MessageTreeNode::writeDataToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("name", m_name);
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
    m_idString = generateIdString(id, IDE, RTR);
}
