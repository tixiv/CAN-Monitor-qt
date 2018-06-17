#ifndef TRANSMITWIDGET_H
#define TRANSMITWIDGET_H

#include <QWidget>
#include <QList>
#include "WidgetUtils/CustomLineEdit.h"
#include "lib-slcan/can_message.h"

namespace Ui {
class TransmitWidget;
}

class TransmitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitWidget(QWidget *parent = 0);
    ~TransmitWidget();

signals:
    void onTransmit(can_message_t cmsg);

private slots:
    void on_transmitButton_clicked();

    void on_IDEcheckBox_toggled(bool checked);

private:
    Ui::TransmitWidget *ui;
    CustomLineEdit * m_idEdit;
    CustomLineEdit * m_dlcEdit;
    QList<CustomLineEdit *> m_dataEdits;

};

#endif // TRANSMITWIDGET_H
