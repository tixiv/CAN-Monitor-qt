#ifndef TRANSMITWIDGET_H
#define TRANSMITWIDGET_H

#include <QWidget>
#include <QList>
#include "WidgetUtils/CustomLineEdit.h"

namespace Ui {
class TransmitWidget;
}

class TransmitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitWidget(QWidget *parent = 0);
    ~TransmitWidget();

private slots:
    void on_transmitButton_clicked();

private:
    Ui::TransmitWidget *ui;
    CustomLineEdit * m_idEdit;
    CustomLineEdit * m_dlcEdit;
    QList<CustomLineEdit *> m_dataEdits;

};

#endif // TRANSMITWIDGET_H
