#ifndef XMLTREENODEFACTORY_H
#define XMLTREENODEFACTORY_H

#include <QString>

class XmlTreeNode;

class XmlTreeNodeFactory
{
public:
    virtual XmlTreeNode * createNode(const QString &xmlName) = 0;  // construct a node of the given xml type name
    virtual QString getName(const XmlTreeNode * node) = 0;        // gets the xml type name for a given node
};

#endif // XMLTREENODEFACTORY_H
