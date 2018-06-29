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

    bool processMessage(uint8_t command, uint8_t subCommand, int32_t value);

    void setSaved();

    struct ParameterData
    {
        uint8_t command;
        uint8_t subCommand;
        int32_t value;
        bool valueRead;
        int32_t newValue;
        bool newValueSet;
        bool needsSave;
    };
    ParameterData getParameterData();


private:
    QVariant m_name;

    uint8_t m_command = 0;
    uint8_t m_subCommand = 0;

    bool m_valueRead = false;
    int32_t m_value = 0;

    bool m_newValueSet = false;
    int32_t m_newValue = 0;

    bool m_needsSave = false;

    QVariant m_unit;

};

#endif // PARAMETERNODE_H
