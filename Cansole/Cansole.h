#ifndef CANSOLE_H
#define CANSOLE_H

#include <QObject>

class QTcpServer;
class QTcpSocket;
class QProcess;

#include "CanHub/CanHub.h"

class Cansole : public QObject
{
    Q_OBJECT

public:
    Cansole(QObject * parent, CanHub &canHub, int cansoleId);
    virtual ~Cansole();

private slots:
    void messageReceived(can_message_t cmsg);

    void clientConected();
    void socketReadyRead();
private:
    CanHandle * m_canHandle = 0;

    int m_cansoleId;

    QProcess * m_puttyProcess = 0;
    QTcpServer * m_tcpServer = 0;
    QTcpSocket * m_tcpSocket = 0;

    bool m_isConnected = false;
};

#endif // CANSOLE_H
