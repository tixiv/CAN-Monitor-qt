#include "XmlTreeNode.h"
#include "XmlTreeNodeFactory.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

// writes a branch including the parent node "node" into a writer
void XmlTreeNode::writeBranchToXml(QXmlStreamWriter &writer, XmlTreeNodeFactory &factory) const
{
    writer.writeStartElement(factory.getName(this));

    writeDataToXml(writer);
    for(int i = 0; i < childCount(); i++){
        static_cast<const XmlTreeNode*>(child(i))->writeBranchToXml(writer, factory);
    }
    writer.writeEndElement();
}

// reads and creates a tree branch and returns a pointer to the root node of the branch
XmlTreeNode * XmlTreeNode::readBranchFromXml(QXmlStreamReader &reader, XmlTreeNodeFactory & factory)
{
    XmlTreeNode * root = 0;
    if(reader.readNextStartElement())
    {
        root = factory.createNode(reader.name().toString());
        if(root)
        {
            root->readDataFromXml(reader);
            XmlTreeNode * node;
            while((node = readBranchFromXml(reader, factory)))
            {
                root->appendChild(node);
            }
        }
        else
            reader.skipCurrentElement();
    }

    return root;
}
