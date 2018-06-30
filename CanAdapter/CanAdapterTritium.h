#ifndef CANADAPTERTRITIUM_H
#define CANADAPTERTRITIUM_H

#include "CanAdapter.h"
#include <QHostAddress>
#include <QUdpSocket>

class CanHub;
class CanHandle;
class TritiumMessage;

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

private slots:
    void openClicked();
    void closeClicked();

    void transmit(can_message_t cmsg);

    void processDatagrams();
private:
    CanHandle * m_canHandle;

    QUdpSocket m_udpSocket;
    QHostAddress m_groupAddress;
    int m_port;
    void processMessage(TritiumMessage &message);
};

#endif // CANADAPTERTRITIUM_H
