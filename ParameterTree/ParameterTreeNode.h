#ifndef PARAMETERTREENODE_H
#define PARAMETERTREENODE_H

#include "Tree/XmlTreeNode.h"

enum parameterColumnFunction
{
    pcf_name,
    pcf_command,
    pcf_subCommand,
    pcf_access,
    pcf_format,
    pcf_value,
    pcf_newValue,
    pcf_unit,
};

class ParameterTreeNode : public XmlTreeNode
{
public:
    virtual QVariant getData(parameterColumnFunction pcf, int role) const = 0;
    virtual bool setData(parameterColumnFunction pcf, const QVariant &value) = 0;
    virtual Qt::ItemFlags getFlags(parameterColumnFunction pcf, bool editMode) const = 0;
};

#endif // PARAMETERTREENODE_H
