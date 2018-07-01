#include "ComboBoxDelegate.h"

#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent, QStringList options)
    : QStyledItemDelegate(parent), m_options(options)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
     QComboBox *editor = new QComboBox(parent);
     editor->addItems(m_options);
     return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
     QString value = index.model()->data(index, Qt::EditRole).toString();
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     comboBox->setCurrentText(value);
     comboBox->showPopup();
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     QString value = comboBox->currentText();
     model->setData(index, value);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
