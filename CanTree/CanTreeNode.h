#ifndef CANTREENODE_H
#define CANTREENODE_H

#include "Tree/XmlTreeNode.h"

enum dataFunction{
    dfName,
    dfID,
    dfDLC,
    dfCount,
    dfPeriod,
    dfRawData,
    dfDataDecoded,
    dfFormat,
};

class CanTreeNode : public XmlTreeNode
{
public:
    virtual QVariant getData(dataFunction df, int role) const = 0;
    virtual bool setData(dataFunction df, const QVariant &value) = 0;
};

#endif // CANTREENODE_H
