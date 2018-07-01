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

    enum AccessMode
    {
        a_readWrite = 0,
        a_readOnly = 1,
        a_writeOnly = 2,
    };

    struct ParameterData
    {
        uint8_t command;
        uint8_t subCommand;
        int32_t value;
        bool valueRead;
        int32_t newValue;
        bool newValueSet;
        bool needsSave;
        AccessMode accessMode;
    };
    ParameterData getParameterData();

    static const QStringList accessStrings;

    Qt::ItemFlags getFlags(parameterColumnFunction pcf, bool editMode) const override;
private:
    QVariant m_name;
    QVariant m_format;

    uint8_t m_command = 0;
    uint8_t m_subCommand = 0;

    bool m_valueRead = false;
    int32_t m_value = 0;

    bool m_newValueSet = false;
    int32_t m_newValue = 0;

    bool m_needsSave = false;

    QVariant m_unit;

    AccessMode m_accessMode = a_readWrite;
};

#endif // PARAMETERNODE_H
