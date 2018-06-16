#ifndef CANADAPTERLAWICEL_H
#define CANADAPTERLAWICEL_H

#include <QString>
#include <QSerialPort>
#include <QTimer>
#include "CanAdapter.h"

class CanAdapterLawicel : public CanAdapter
{
public:
    CanAdapterLawicel();

    bool open(int baudrate) override;
    void close() override;

    bool transmit(const can_message_t * cmsg) override;
    bool receive(can_message_t * cmsg) override;

    bool isOpen() override;

private slots:
    void openTimerTimeout();

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

};

#endif // CANADAPTERLAWICEL_H
