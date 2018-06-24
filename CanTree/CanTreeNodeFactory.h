#ifndef CANTREENODEFACTORY_H
#define CANTREENODEFACTORY_H

#include "Tree/XmlTreeNodeFactory.h"

class CanTreeNodeFactory : public XmlTreeNodeFactory
{
public:
    XmlTreeNode * createNode(const QString &xmlName) override;
    QString getName(const XmlTreeNode * node) override;
};

#endif // CANTREENODEFACTORY_H
