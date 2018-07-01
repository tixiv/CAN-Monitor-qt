#include "ParameterNode.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QColor>

const QStringList ParameterNode::accessStrings = QStringList()
    << "Read/Write"
    << "Read only"
    << "Write only";

ParameterNode::ParameterNode(QVariant name)
{
    m_name = name;
}

QVariant ParameterNode::getData(parameterColumnFunction pcf, int role) const
{

    if(role == Qt::BackgroundRole)
    {
        if( (pcf == pcf_newValue && m_accessMode == a_readOnly)||
            (pcf == pcf_value && m_accessMode == a_writeOnly) )
            return QVariant(QColor(Qt::lightGray));
        else
            return QVariant();
    }

    switch(pcf)
    {
    case pcf_name: return m_name;
    case pcf_command: return QString().number((uint)m_command, 16);
    case pcf_subCommand: return QString().number((uint)m_subCommand, 16);
    case pcf_access: return accessStrings.at(m_accessMode);
    case pcf_format: return m_format;
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

Qt::ItemFlags ParameterNode::getFlags(parameterColumnFunction pcf, bool editMode) const
{
    if(pcf == pcf_newValue)
        return m_accessMode != a_readOnly ? Qt::ItemIsEditable : Qt::NoItemFlags;

    if(editMode)
    {
        if(pcf != pcf_value)
            return Qt::ItemIsEditable;
    }

    return Qt::NoItemFlags;
}

bool ParameterNode::setData(parameterColumnFunction pcf, const QVariant &value)
{
    switch(pcf)
    {
    case pcf_name: m_name = value; break;
    case pcf_command: m_command = value.toString().toInt(0,16); break;
    case pcf_subCommand: m_subCommand = value.toString().toInt(0,16); break;
    case pcf_access: m_accessMode = (AccessMode)accessStrings.indexOf(value.toString()); break;
    case pcf_format: m_format = value; break;
    case pcf_newValue:
        {
            bool ok = false;
            int32_t val = value.toString().toInt(&ok);
            if(ok){
                m_newValue = val;
                m_newValueSet = true;
                m_needsSave = true;
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
    writer.writeAttribute("accessMode", QString().number(m_accessMode));
    writer.writeAttribute("format", m_format.toString());
    writer.writeAttribute("unit", m_unit.toString());

}

void ParameterNode::readDataFromXml(QXmlStreamReader &reader)
{
    m_name = reader.attributes().value("name").toString();
    m_command = reader.attributes().value("command").toString().toInt(0,16);
    m_subCommand = reader.attributes().value("subCommand").toString().toInt(0,16);
    m_accessMode = (AccessMode)reader.attributes().value("accessMode").toString().toInt();
    m_format = reader.attributes().value("format").toString();
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

void ParameterNode::setSaved()
{
    m_needsSave = false;
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
    pd.needsSave = m_needsSave;
    pd.accessMode = m_accessMode;
    return pd;
}
