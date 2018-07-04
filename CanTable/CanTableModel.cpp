#include "CanTableModel.h"

CanTableModel::CanTableModel()
    : TreeModel(new CanTableNode())
{
    m_columnFunctions = {
        ColumnRole(ctf_name,        "Name"),
    };
    m_columnCount = m_columnFunctions.count();
}

int CanTableModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_columnCount;
}

QVariant CanTableModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_columnFunctions.at(section).name;
    }

    return QVariant();
}

QVariant CanTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole))
        return QVariant();

    CanTableNode *node = static_cast<CanTableNode*>(nodeForIndex(index));

    return node->getData(m_columnFunctions.at(index.column()).cf, role);
}

Qt::ItemFlags CanTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = 0;
    switch(m_columnFunctions.at(index.column()).cf){
    case ctf_name:
        break;
    }

    return flags | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
