#include "ParameterTreeNodeFactory.h"
#include "ParameterGroupNode.h"
#include "ParameterNode.h"

XmlTreeNode * ParameterTreeNodeFactory::createNode(const QString &xmlName)
{
    if(xmlName == "ParameterGroupNode"){
        return new ParameterGroupNode();
    }else if(xmlName == "ParameterNode"){
        return new ParameterNode();
    }else
        return 0;
}

QString ParameterTreeNodeFactory::getName(const XmlTreeNode * node)
{
    if(dynamic_cast<const ParameterGroupNode *>(node)){
        return "ParameterGroupNode";
    }else if(dynamic_cast<const ParameterNode *>(node)){
        return "ParameterNode";
    }else
        return "";
}
