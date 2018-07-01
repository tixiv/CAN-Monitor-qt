#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QWidget>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QStyledItemDelegate>

class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(QObject *parent, QStringList options);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QStringList m_options;
};

#endif // COMBOBOXDELEGATE_H
