#ifndef CANTABLENODE_H
#define CANTABLENODE_H

#include "Tree/XmlTreeNode.h"

enum CanTableColumnFunction
{
    ctf_name,

};

class CanTableNode : public XmlTreeNode
{
public:
    CanTableNode();
    virtual QVariant getData(CanTableColumnFunction cf, int role) const;
    virtual void writeDataToXml(QXmlStreamWriter &writer) const;
    virtual void readDataFromXml(QXmlStreamReader &reader);
    bool acceptsChildren() const override;
};

#endif // CANTABLENODE_H
