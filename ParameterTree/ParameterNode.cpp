#include "ParameterNode.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

ParameterNode::ParameterNode(QVariant name)
{
    m_name = name;
}

QVariant ParameterNode::getData(parameterColumnFunction pcf, int role) const
{
    (void) role;
    switch(pcf)
    {
    case pcf_name: return m_name;
    case pcf_command: return QString().number((uint)m_command, 16);
    case pcf_subCommand: return QString().number((uint)m_subCommand, 16);
    case pcf_value:
        if(m_valueRead)
            return QString().number(m_value);
        else
            return QVariant();
    case pcf_newValue:
        if(m_newValueSet)
            return QString().number(m_newValue);
        else if(role == Qt::EditRole && m_valueRead)
            return QString().number(m_value);
        else
            return QVariant();
    case pcf_unit: return m_unit;
    }

    return QVariant();
}

bool ParameterNode::setData(parameterColumnFunction pcf, const QVariant &value)
{
    switch(pcf)
    {
    case pcf_name: m_name = value; break;
    case pcf_command: m_command = value.toString().toInt(0,16); break;
    case pcf_subCommand: m_subCommand = value.toString().toInt(0,16); break;
    case pcf_newValue:
        {
            bool ok = false;
            int32_t val = value.toString().toInt(&ok);
            if(ok){
                m_newValue = val;
                m_newValueSet = true;
            }
        }
        break;
    case pcf_unit: m_unit = value; break;

    default: return false;
    }

    return true;
}

void ParameterNode::writeDataToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("name", m_name.toString());
    writer.writeAttribute("command", QString().number((uint)m_command, 16));
    writer.writeAttribute("subCommand", QString().number((uint)m_subCommand, 16));
    writer.writeAttribute("unit", m_unit.toString());

}

void ParameterNode::readDataFromXml(QXmlStreamReader &reader)
{
    m_name = reader.attributes().value("name").toString();
    m_command = reader.attributes().value("command").toString().toInt(0,16);
    m_subCommand = reader.attributes().value("subCommand").toString().toInt(0,16);
    m_unit = reader.attributes().value("unit").toString();
}

bool ParameterNode::processMessage(uint8_t command, uint8_t subCommand, int32_t value)
{
    if(command == m_command && subCommand == m_subCommand)
    {
        m_value = value;
        m_valueRead = true;
        return true;
    }
    else
    {
        return false;
    }
}

ParameterNode::ParameterData ParameterNode::getParameterData()
{
    ParameterData pd;
    pd.command = m_command;
    pd.subCommand = m_subCommand;
    pd.value = m_value;
    pd.valueRead = m_valueRead;
    pd.newValue = m_newValue;
    pd.newValueSet = m_newValueSet;
    return pd;
}
