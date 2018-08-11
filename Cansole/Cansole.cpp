#include "Cansole.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <QMessageBox>
#include <QThread>

Cansole::Cansole(QObject * parent, CanHub &canHub, int cansoleId)
    : QObject(parent), m_cansoleId(cansoleId)
{
    m_canHandle = canHub.getNewHandle();
    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(messageReceived(can_message_t)));

    // TCP Server that putty connects to
    m_tcpServer = new QTcpServer();
    m_tcpServer->listen(QHostAddress(QHostAddress::LocalHost));
    connect(m_tcpServer, &QTcpServer::newConnection, this, &Cansole::clientConected);

    // Start Putty telnet
    m_puttyProcess = new QProcess(this);
    QString cmd = QString("putty -telnet -P %1 localhost").arg(m_tcpServer->serverPort());
    connect(m_puttyProcess, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    m_puttyProcess->start(cmd);
    m_puttyProcess->waitForStarted();

    if(m_puttyProcess->state() != QProcess::Running)
    {
        QMessageBox::warning(0, tr("CAN Monitor"),
                             tr("Putty could not be started"),
                             QMessageBox::Ok);
        deleteLater();
    }
}

Cansole::~Cansole()
{
    m_isConnected = false;
    m_puttyProcess->close();
    delete m_puttyProcess;
    delete m_tcpSocket;
    delete m_tcpServer;
    delete m_canHandle;
}

void Cansole::messageReceived(can_message_t cmsg)
{
    if(m_isConnected &&
            cmsg.id == m_cansoleId &&
            cmsg.dlc > 1           &&
            cmsg.data[0] == 0x37)
    {
        QByteArray ba((char*)&cmsg.data[1], cmsg.dlc-1);
        m_tcpSocket->write(ba);
    }
}

void Cansole::socketReadyRead()
{
    QByteArray ba = m_tcpSocket->readAll();

    // qDebug() << ba;

    // remove telnet control sequences
    while((uint8_t)ba[0] == 0xff)
        ba = ba.mid(3);


    while(ba.length() > 0)
    {
        int len=7;
        if(ba.length() < 7)
            len = ba.length();

        can_message_t cmsg;
        cmsg.IDE = 0;
        cmsg.RTR = 0;
        cmsg.id = m_cansoleId + 1;
        cmsg.dlc = len + 1;
        cmsg.data[0] = 0x37;
        memcpy(&cmsg.data[1], ba.data(), len);
        m_canHandle->transmit(cmsg);

        ba = ba.mid(len);
    }
}

void Cansole::clientConected()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    connect(m_tcpSocket, &QAbstractSocket::disconnected, this, &QObject::deleteLater);
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

    QByteArray data;

    // Set terminal mode
    data.append("\xFF\xFB\x01"); // Telnet IAC WILL ECHO
    data.append("\xFF\xFB\x03"); // Telnet IAC WILL SUPPRESS_GO_AHEAD
    data.append("\xFF\xFC\x22"); // Telnet IAC WONT LINEMODE

    // Set window title
    data.append(QString().sprintf("\033]0;" "Cansole %X" "\007", m_cansoleId));

    m_tcpSocket->write(data);


    m_isConnected = true;
}
