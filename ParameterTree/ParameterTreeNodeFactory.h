#ifndef PARAMETERTREENODEFACTORY_H
#define PARAMETERTREENODEFACTORY_H

#include "Tree/XmlTreeNodeFactory.h"

class ParameterTreeNodeFactory : public XmlTreeNodeFactory
{
public:
    XmlTreeNode * createNode(const QString &xmlName) override;
    QString getName(const XmlTreeNode * node) override;
};

#endif // PARAMETERTREENODEFACTORY_H
