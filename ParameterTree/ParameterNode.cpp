#include "ParameterNode.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

ParameterNode::ParameterNode()
{

}

QVariant ParameterNode::getData(parameterColumnFunction pcf, int role) const
{
    (void) role;
    if(pcf == pcf_name){
        return m_name;
    }else{
        return QVariant();
    }
}

bool ParameterNode::setData(parameterColumnFunction pcf, const QVariant &value)
{
    if(pcf == pcf_name){
        m_name = value;
        return true;
    }else{
        return false;
    }
}

void ParameterNode::writeDataToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("name", m_name.toString());
}

void ParameterNode::readDataFromXml(QXmlStreamReader &reader)
{
    m_name = reader.attributes().value("name").toString();
}
