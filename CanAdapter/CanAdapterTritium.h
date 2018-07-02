#ifndef CANADAPTERTRITIUM_H
#define CANADAPTERTRITIUM_H

#include "CanAdapter.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QTimer>

class CanHub;
class CanHandle;
class TritiumMessage;
class TritiumHeader;

class CanAdapterTritium : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterTritium(CanHub &canHub);
    ~CanAdapterTritium() override;

    bool open() override;
    void close() override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

signals:
    void updateStatus(QString text);

private slots:
    void openClicked();
    void closeClicked();

    void transmit(can_message_t cmsg);

    void processDatagrams();
    void statusTimerTimeout();
private:
    CanHandle * m_canHandle;

    bool m_isOpen = false;

    QUdpSocket m_udpSocket;
    QHostAddress m_groupAddress;
    int m_port;
    void processMessage(const TritiumMessage &message, const TritiumHeader &header, const QHostAddress &sourceAddress);
    void generateClientIdentifier();

    char m_clientIdentifier[7] = {0};

    QTimer m_statusTimer;
};

#endif // CANADAPTERTRITIUM_H
