#include "CanAdapterChina.h"

#include <QMessageBox>
#include "lib-slcan/slcan.h"
#include "ChinaControlWidget.h"

CanAdapterChina::CanAdapterChina(CanHub &canHub)
    : m_portName("COM8"), m_uartBaudRate(2000000), m_openState(osClosed)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(transmit(can_message_t)));
    connect(&m_openTimer, SIGNAL(timeout()), this, SLOT(openTimerTimeout()));
    connect(&m_port, SIGNAL(readyRead()), this, SLOT(readBytesReady()) );
}

CanAdapterChina::~CanAdapterChina(){
    close();
    delete m_canHandle;
}

bool CanAdapterChina::open()
{
    if(m_openState != osClosed)
        return false;

    m_port.setPortName(m_portName);
    m_port.setBaudRate(m_uartBaudRate);
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setFlowControl(QSerialPort::NoFlowControl);

    if(m_port.open(QIODevice::ReadWrite)){
        m_openState = osOpening;
        m_openTimer.setInterval(100);
        m_openTimer.setSingleShot(true);
        m_openTimer.start();

    }else{
        QMessageBox::warning(0, tr("CAN Monitor"),
                             tr("The serial port could not be opened.\n"
                             "The errormessage was:\n")
                             +m_port.errorString(),
                             QMessageBox::Ok);
        emit openOperationEnded(false);
        return false;
    }

    return true;
}

static QByteArray build_packet(const uint8_t *data, int len)
{
    QByteArray ba(len+3, 0);
    ba[0] = 0xaa;
    ba[1] = 0x55;

    uint8_t checksumm = 0;
    for(int i=0; i<len; i++)
    {
        ba[2+i]    = data[i];
        checksumm += data[i];
    }

    ba[len+2] = checksumm;
    return ba;
}

static uint8_t convert_baud(int baud)
{
    switch(baud)
    {
    case 1000000: return  1;
    case  800000: return  2;
    case  500000: return  3;
    case  400000: return  4;
    case  250000: return  5;
    case  200000: return  6;
    case  125000: return  7;
    case  100000: return  8;
    case   50000: return  9;
    case   20000: return 10;
    case   10000: return 11;
    case    5000: return 12;
    default  : return 7; // 125k
    }

}


// Open From new software
// aa 55 12 05 01 00 00 00 00 00 00 00 00 00 01 00 00 00 00 19   Open normal    standard  250kBaud
// aa 55 12 07 01 00 00 00 00 00 00 00 00 00 01 00 00 00 00 1b   Open normal    standard  125kBaud
// aa 55 12 07 01 00 00 00 00 00 00 00 00 01 01 00 00 00 00 1c   Open loopback  standard  125kBaud
// aa 55 12 07 01 00 00 00 00 00 00 00 00 02 01 00 00 00 00 1d   Open silent    standard  125kBaud
// aa 55 12 07 01 00 00 00 00 00 00 00 00 03 01 00 00 00 00 1e   Open sl+lb     standard  125kBaud
// aa 55 12 07 02 00 00 00 00 00 00 00 00 03 01 00 00 00 00 1f   Open sl+lb     extended  125kBaud

// Open From old software
// AA 55 02 07 02 00 00 00 00 00 00 00 00 00 01 00 00 00 00 0C   Open normal    extended  125kBaud
// AA 55 02 07 01 00 00 00 00 00 00 00 00 00 01 00 00 00 00 0B   Open normal    std+ext   125kBaud

static QByteArray build_config_packet(int baud, int mode)
{
    uint8_t packet[17] = {0x02};
    packet[1] = convert_baud(baud);
    packet[2] = 1; // Std+ext frames (2 = ext only)
    packet[11] = mode; // 0=normal, 1=loopback, 2=silent, 3=sl+lb
    packet[12] = 1; // ?

    return build_packet(packet, 17);
}

// Packets from new software
// AA E8 23 01 00 00 11 22 33 44 55 66 77 88 55   // Ext ID 00000123  11 22 33 44 55 66 77 88
// AA C8 23 01 11 22 33 44 55 66 77 88 55         // Std ID      123  11 22 33 44 55 66 77 88
// AA C4 23 01 11 22 33 44 55                     // Std ID      123  11 22 33 44

// Packets from old software
// AA 55 01 02 01 23 01 00 00 04 11 22 33 44 00 00 00 00 00 D6  // Ext ID      123  11 22 33 44
// AA 55 01 02 02 23 01 00 00 04 11 22 33 44 00 00 00 00 00 D7  // Ext ID      123  RTR
// AA 55 01 01 01 23 01 00 00 04 11 22 33 44 00 00 00 00 00 D5  // Std ID      123  11 22 33 44

static QByteArray build_can_packet(const can_message_t *cmsg)
{
    uint8_t packet[17] = {0x01};
    packet[1] = cmsg->IDE ? 0x02 : 0x01;
    packet[2] = cmsg->RTR ? 0x02 : 0x01;
    packet[3] = (cmsg->id >>  0) & 0xff;
    packet[4] = (cmsg->id >>  8) & 0xff;
    packet[5] = (cmsg->id >> 16) & 0xff;
    packet[6] = (cmsg->id >> 24) & 0xff;
    packet[7] = cmsg->dlc;

    if(!cmsg->RTR)
        memcpy(&packet[8], cmsg->data, cmsg->dlc);

    return build_packet(packet, 17);
}

static bool parse_can_packet(can_message_t *cmsg, const QByteArray &packet)
{
    uint8_t checksumm = 0;
    for(int i=2; i<19; i++)
        checksumm += (uint8_t)packet[i];

    if(checksumm != (uint8_t)packet[19])         // checksumm error
        return false;

    if(packet[2] != 0x01)  // no can packet
        return false;

    // bad identifier or rtr check
    if(     (packet[3] != 0x01 && packet[3] != 0x02) ||
            (packet[4] != 0x01 && packet[4] != 0x02)  )
        return false;

    if((uint8_t)packet[9] > 8) // dlc check
        return false;

    cmsg->IDE = packet[3] == 0x02;
    cmsg->RTR = packet[4] == 0x02;

    cmsg->id  = ((uint8_t)packet[5]) <<  0;
    cmsg->id |= ((uint8_t)packet[6]) <<  8;
    cmsg->id |= ((uint8_t)packet[7]) << 16;
    cmsg->id |= ((uint8_t)packet[8]) << 24;

    cmsg->dlc = packet[9];
    memcpy(cmsg->data, &packet.data()[10], cmsg->dlc);

    return true;
}


void CanAdapterChina::openTimerTimeout()
{
    m_port.write(build_config_packet(m_canBaudRate, m_mode));
    m_openState = osOpen;

    emit openOperationEnded(true);
}

void CanAdapterChina::close()
{
    if(m_openState != osClosed)
    {
        m_openTimer.stop();
        m_port.write("C\r");
        m_port.waitForBytesWritten(100);
        m_port.close();
        m_openState = osClosed;
    }
}

void CanAdapterChina::transmit(can_message_t cmsg)
{
    if(m_openState != osOpen)
        return;

    m_port.write(build_can_packet(&cmsg));
}

bool CanAdapterChina::receive(can_message_t * cmsg)
{
    if(m_openState != osOpen)
        return false;

    m_buffer.append(m_port.readAll());

    bool got_message = false;
    while(!got_message && m_buffer.length() >= 20)
    {
        int pos = m_buffer.indexOf("\xaa\x55");
        if(pos >= 0)
        {
            m_buffer = m_buffer.mid(pos);
            if(m_buffer.length() >= 20)
            {
                if(parse_can_packet(cmsg, m_buffer))
                {
                    got_message = true;
                    m_buffer = m_buffer.mid(20);
                }
                else
                {
                    m_buffer = m_buffer.mid(2);
                }
            }
        }
        else
        {
            // there is something wrong if we got so many bytes without sync.
            if(m_buffer.length() > 100){
                m_buffer.clear();    //clear buffer so it doesn't overflow
            }
            break;
        }
    }

    return got_message;
}

void CanAdapterChina::readBytesReady()
{
    can_message_t cmsg;
    while(receive(&cmsg)){
        m_canHandle->transmit(cmsg);
    }
}

bool CanAdapterChina::isOpen()
{
    return m_openState == osOpen;
}

QWidget * CanAdapterChina::getControlWidget(QWidget *parent){
    auto controlWidget = new ChinaControlWidget(parent);
    connect(controlWidget, SIGNAL(openClicked(QString, CanAdapterChina::OpenMode, int)), this, SLOT(openClicked(QString, CanAdapterChina::OpenMode, int)));
    connect(controlWidget, SIGNAL(closeClicked()), this, SLOT(closeClicked()));
    connect(this, SIGNAL(openOperationEnded(bool)), controlWidget, SLOT(openOperationEnded(bool)));

    return controlWidget;
}

void CanAdapterChina::openClicked(QString portName, CanAdapterChina::OpenMode mode, int baud){
    m_portName = portName;
    m_canBaudRate = baud;
    m_mode = mode;
    open();
}

void CanAdapterChina::closeClicked(){
    close();
}
