#ifndef CANADAPTERLAWICEL_H
#define CANADAPTERLAWICEL_H

#include <QString>
#include <QSerialPort>
#include <QTimer>
#include "CanAdapter.h"

class CanAdapterLawicel : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterLawicel();
    ~CanAdapterLawicel() override;

    bool open() override;
    void close() override;

    bool transmit(const can_message_t * cmsg) override;
    bool receive(can_message_t * cmsg) override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

signals:
    void openOperationEnded(bool success);

private slots:
    void openTimerTimeout();

    void openClicked(QString portName, QString mode, int baud);
    void closeClicked();

private:
    QSerialPort m_port;
    QTimer m_openTimer;

    QString m_portName;
    int m_uartBaudRate;

    int m_canBaudRate;

    enum{
        osClosed,
        osOpening,
        osOpen
    }m_openState;

    QString m_buffer;

    QWidget * m_controlWidget=0;

};

#endif // CANADAPTERLAWICEL_H
