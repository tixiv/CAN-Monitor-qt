#ifndef CANTABLEMODEL_H
#define CANTABLEMODEL_H

#include "CanTableNode.h"
#include "Tree/TreeModel.h"

class CanTableModel : public TreeModel
{
    Q_OBJECT

public:
    CanTableModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    //void writeTreeToXml(QXmlStreamWriter &writer);
    //bool readTreeFromXml(QXmlStreamReader &reader);

//    void inputMessage(can_message_t cmsg);

private:
    struct ColumnRole {
        ColumnRole(CanTableColumnFunction cf, const char * name):cf(cf), name(name){}
        CanTableColumnFunction cf;
        QString name;
    };
    QVector<ColumnRole> m_columnFunctions;
    int m_columnCount;
};

#endif // CANTABLEMODEL_H
