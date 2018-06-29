#ifndef PARAMETERTREEMODEL_H
#define PARAMETERTREEMODEL_H

#include "Tree/TreeModel.h"
#include "ParameterTreeNode.h"
#include <QStringList>

class CanHandle;

class ParameterTreeModel : public TreeModel
{
    Q_OBJECT

public:
    ParameterTreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void writeTreeToXml(QXmlStreamWriter &writer);
    bool readTreeFromXml(QXmlStreamReader &reader);

    void inputMessage(uint8_t command, uint8_t subCommand, int32_t value);
    void setEditMode(bool active);
signals:
    void newValueEdited(ParameterTreeNode * node);

private:
    struct ColumnRole {
        ColumnRole(parameterColumnFunction df, const char * name):df(df), name(name){}
        parameterColumnFunction df;
        QString name;
    };
    QVector<ColumnRole> m_columnFunctions;

    bool m_editModeActive = false;
};

#endif // PARAMETERTREEMODEL_H
