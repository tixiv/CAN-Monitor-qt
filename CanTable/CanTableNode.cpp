#include "CanTableNode.h"


CanTableNode::CanTableNode()
{

}

QVariant CanTableNode::getData(CanTableColumnFunction cf, int role) const
{
    return "foo";
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
