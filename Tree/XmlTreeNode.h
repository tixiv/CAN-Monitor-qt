#ifndef XMLTREENODE_H
#define XMLTREENODE_H

#include "TreeNode.h"

class QXmlStreamWriter;
class QXmlStreamReader;
class XmlTreeNodeFactory;

class XmlTreeNode : public TreeNode
{
public:
    // reads and creates a tree branch and returns a pointer to the root node of the branch
    static XmlTreeNode * readBranchFromXml(QXmlStreamReader &reader, XmlTreeNodeFactory & factory);

    // writes a branch including the parent node "node" into a writer
    void writeBranchToXml(QXmlStreamWriter &writer, XmlTreeNodeFactory &factory) const;

protected:
    // child classes need to impement these.
    // they should write/read their members to/from Xml properties
    virtual void writeDataToXml(QXmlStreamWriter &writer) const = 0;
    virtual void readDataFromXml(QXmlStreamReader &reader) = 0;
};

#endif // XMLTREENODE_H
