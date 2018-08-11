#ifndef CANADAPTERCHINA_H
#define CANADAPTERCHINA_H

#include <QString>
#include <QSerialPort>
#include <QTimer>
#include "CanAdapter.h"
#include "CanHub/CanHub.h"

class CanAdapterChina : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterChina(CanHub &canHub);
    ~CanAdapterChina() override;

    bool open() override;
    void close() override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

    enum OpenMode{
        om_normal = 0,
        om_loopback = 1,
        om_listeOnly = 2,
        om_listeOnlyLoopback = 3,
    };

signals:
    void openOperationEnded(bool success);

private slots:
    void openTimerTimeout();

    void openClicked(QString portName, CanAdapterChina::OpenMode mode, int baud);
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

    QByteArray m_buffer;
};

#endif // CANADAPTERCHINA_H
