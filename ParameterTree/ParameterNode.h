#ifndef PARAMETERNODE_H
#define PARAMETERNODE_H

#include "ParameterTreeNode.h"

class ParameterNode : public ParameterTreeNode
{
public:
    ParameterNode();

    QVariant getData(parameterColumnFunction pcf, int role) const override;
    bool setData(parameterColumnFunction pcf, const QVariant &value) override;

    void writeDataToXml(QXmlStreamWriter &writer) const override;
    void readDataFromXml(QXmlStreamReader &reader) override;

private:
    QVariant m_name;
};

#endif // PARAMETERNODE_H
