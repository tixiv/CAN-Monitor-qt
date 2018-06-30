#ifndef CANADAPTERLAWICEL_H
#define CANADAPTERLAWICEL_H

#include <QString>
#include <QSerialPort>
#include <QTimer>
#include "PollingCanAdapter.h"

class CanAdapterLawicel : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterLawicel(CanHub &canHub);
    ~CanAdapterLawicel() override;

    bool open() override;
    void close() override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

    enum OpenMode{
        om_normal,
        om_listeOnly,
        om_loopback,
    };

signals:
    void openOperationEnded(bool success);

private slots:
    void openTimerTimeout();

    void openClicked(QString portName, CanAdapterLawicel::OpenMode mode, int baud);
    void closeClicked();

    void transmit(can_message_t cmsg);

    void readBytesReady();

private:
    CanHandle * m_canHandle;

    bool receive(can_message_t * cmsg);

    QSerialPort m_port;
    QTimer m_openTimer;

    QString m_portName;
    int m_uartBaudRate;

    int m_canBaudRate;
    OpenMode m_mode;

    enum{
        osClosed,
        osOpening,
        osOpen
    }m_openState;

    QString m_buffer;
};

#endif // CANADAPTERLAWICEL_H
