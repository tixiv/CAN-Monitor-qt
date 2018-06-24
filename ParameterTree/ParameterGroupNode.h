#ifndef PARAMETERGROUPNODE_H
#define PARAMETERGROUPNODE_H

#include "ParameterTreeNode.h"

class ParameterGroupNode : public ParameterTreeNode
{
public:
    ParameterGroupNode(const QVariant &name="");
    bool dropAllowed() const override;

    QVariant getData(parameterColumnFunction pcf, int role) const override;
    bool setData(parameterColumnFunction pcf, const QVariant &value) override;

    void writeDataToXml(QXmlStreamWriter &writer) const override;
    void readDataFromXml(QXmlStreamReader &reader) override;

private:
    QVariant m_name;
};

#endif // PARAMETERGROUPNODE_H
