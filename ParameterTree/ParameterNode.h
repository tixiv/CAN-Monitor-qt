#ifndef PARAMETERNODE_H
#define PARAMETERNODE_H

#include "ParameterTreeNode.h"

class ParameterNode : public ParameterTreeNode
{
public:
    ParameterNode(QVariant name="");

    QVariant getData(parameterColumnFunction pcf, int role) const override;
    bool setData(parameterColumnFunction pcf, const QVariant &value) override;

    void writeDataToXml(QXmlStreamWriter &writer) const override;
    void readDataFromXml(QXmlStreamReader &reader) override;

private:
    QVariant m_name;

    uint8_t m_command = 0;
    uint8_t m_subCommand = 0;

    bool m_valueRead = false;
    int32_t m_value = 0;

    bool m_newValueSet = false;
    int32_t m_newValue = 0;

    QVariant m_unit;

};

#endif // PARAMETERNODE_H
