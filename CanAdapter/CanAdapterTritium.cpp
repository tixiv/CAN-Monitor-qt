#include "CanAdapterTritium.h"

#include "lib-slcan/slcan.h"
#include "SlcanControlWidget.h"
#include <QDebug>

CanAdapterTritium::CanAdapterTritium(CanHub &canHub)
    : m_groupAddress(QStringLiteral("239.255.60.60")), m_port(4876)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(transmit(can_message_t)));
    open();
}

CanAdapterTritium::~CanAdapterTritium(){
    close();
    delete m_canHandle;
}

bool CanAdapterTritium::open()
{
    m_udpSocket.bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress);
    m_udpSocket.joinMulticastGroup(m_groupAddress);
    connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
    return true;
}

void CanAdapterTritium::close()
{
}


struct TritiumHeader
{
    uint8_t busNumber;
    char clientIdentifier[7];
};

struct TritiumMessage
{
    uint32_t id;
    bool H;
    bool S;
    bool R;
    bool E;
    uint8_t dlc;
    uint8_t data[8];
};

// Protocoll:
// https://tritium.com.au/includes/TRI82.007v4_Ethernet_Interface.pdf

static bool decodeHeader(TritiumHeader &h, const char * data)
{
    if(memcmp(&data[1], "Tritiu", 6)) // check magic (first 6 bytes)
        return false;
    if((data[7] & 0xF0) != 0x60) // check magic (last nibble)
        return false;

    h.busNumber = data[7] & 0x0F;
    memcpy(h.clientIdentifier, &data[9], 7);

    return true;
}

static void decodeMessage(TritiumMessage &m, const char * data)
{
    m.id  = data[0] << 24;
    m.id |= data[1] << 16;
    m.id |= data[2] <<  8;
    m.id |= data[3] <<  0;
    m.H = data[4] & 0x80 ? 1:0;
    m.S = data[4] & 0x40 ? 1:0;
    m.R = data[4] & 0x02 ? 1:0;
    m.E = data[4] & 0x01 ? 1:0;
    m.dlc = data[5];
    memcpy(m.data, &data[6], 8);
}

void CanAdapterTritium::processMessage(TritiumMessage &message)
{
    if(message.H) // heartbeat
    {
    }
    else if(message.S) // setting
    {
    }
    else // CAN message
    {
        can_message_t cmsg;
        cmsg.IDE = message.E;
        cmsg.RTR = message.R;
        cmsg.id = message.id;
        cmsg.dlc = message.dlc;
        memcpy(cmsg.data, message.data, 8);
        m_canHandle->transmit(cmsg);
    }
}

void CanAdapterTritium::processDatagrams()
{
    QByteArray data;

    while (m_udpSocket.hasPendingDatagrams()) {
        data.resize(int(m_udpSocket.pendingDatagramSize()));
        m_udpSocket.readDatagram(data.data(), data.size());
        // qDebug() << data.toHex();
        if(data.length() < 30) // minimum datagram length
            continue;

        const char * d = data.data();
        TritiumHeader header;
        if(!decodeHeader(header, d))
            continue;

        int offset = 16;
        int length = data.length() - 16;
        while (length >= 14) {
            TritiumMessage message;
            decodeMessage(message, &d[offset]);
            processMessage(message);

            offset += 14;
            length -= 14;
        }
    }
}

void CanAdapterTritium::transmit(can_message_t cmsg)
{
}

bool CanAdapterTritium::isOpen()
{
    return true;
}

QWidget * CanAdapterTritium::getControlWidget(QWidget *parent){
    return 0;
}

void CanAdapterTritium::openClicked(){
    open();
}

void CanAdapterTritium::closeClicked(){
    close();
}
