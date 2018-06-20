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

private:
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
