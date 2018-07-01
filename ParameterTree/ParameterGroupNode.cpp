#include "ParameterGroupNode.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

ParameterGroupNode::ParameterGroupNode(const QVariant &name)
    : ParameterTreeNode(), m_name(name)
{
}

bool ParameterGroupNode::acceptsChildren() const
{
    return true;
}

QVariant ParameterGroupNode::getData(parameterColumnFunction pcf, int role) const
{
    if(role == Qt::BackgroundRole)
        return QVariant();

    if(pcf == pcf_name){
        return m_name;
    }else{
        return QVariant();
    }
}

bool ParameterGroupNode::setData(parameterColumnFunction pcf, const QVariant &value)
{
    if(pcf == pcf_name){
        m_name = value;
        return true;
    }else{
        return false;
    }
}

Qt::ItemFlags ParameterGroupNode::getFlags(parameterColumnFunction pcf, bool editMode) const
{
    if(pcf == pcf_name && editMode)
        return Qt::ItemIsEditable;

    return 0;
}

void ParameterGroupNode::writeDataToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("name", m_name.toString());
}

void ParameterGroupNode::readDataFromXml(QXmlStreamReader &reader)
{
    m_name = reader.attributes().value("name").toString();
}
