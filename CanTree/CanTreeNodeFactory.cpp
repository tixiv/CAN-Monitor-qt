#include "CanTreeNodeFactory.h"
#include "HeaderTreeNode.h"
#include "MessageTreeNode.h"

XmlTreeNode * CanTreeNodeFactory::createNode(const QString &xmlName)
{
    if(xmlName == "HeaderTreeNode"){
        return new HeaderTreeNode();
    }else if(xmlName == "MessageTreeNode"){
        return new MessageTreeNode();
    }else
        return 0;
}

QString CanTreeNodeFactory::getName(const XmlTreeNode * node)
{
    if(dynamic_cast<const HeaderTreeNode *>(node)){
        return "HeaderTreeNode";
    }else if(dynamic_cast<const MessageTreeNode *>(node)){
        return "MessageTreeNode";
    }else
        return "";
}
