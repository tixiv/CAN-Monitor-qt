#ifndef HEADERTREENODE_H
#define HEADERTREENODE_H

#include "CanTreeNode.h"

class HeaderTreeNode : public CanTreeNode
{
public:
    HeaderTreeNode(const QVariant &name="");
    bool acceptsChildren() const override;

    QVariant getData(dataFunction df, int role) const override;
    bool setData(dataFunction df, const QVariant &value) override;

    void writeDataToXml(QXmlStreamWriter &writer) const override;
    void readDataFromXml(QXmlStreamReader &reader) override;

private:
    QVariant m_name;
};

#endif // HEADERTREENODE_H
