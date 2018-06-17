#include "CanAdapterLawicel.h"

#include <QThread>
#include "lib-slcan/slcan.h"
#include "SlcanControlWidget.h"

CanAdapterLawicel::CanAdapterLawicel()
    : m_portName("COM4"), m_uartBaudRate(1000000), m_openState(osClosed)
{
    connect(&m_openTimer, &QTimer::timeout, this, &CanAdapterLawicel::openTimerTimeout);
}

CanAdapterLawicel::~CanAdapterLawicel(){
    if(m_controlWidget) delete m_controlWidget;
}

bool CanAdapterLawicel::open()
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
    }

    m_openTimer.setInterval(2000);
    m_openTimer.setSingleShot(true);
    m_openTimer.start();

    return true;
}

void CanAdapterLawicel::openTimerTimeout()
{
    m_port.write(slcan_get_baud_string(m_canBaudRate));
    switch(m_mode){
    case om_normal:
        m_port.write("O\r"); break;
    case om_listeOnly:
        m_port.write("L\r"); break;
    case om_loopback:
        m_port.write("l\r"); break;
    }
    m_openState = osOpen;

    emit openOperationEnded(true);
}

void CanAdapterLawicel::close()
{
    m_openTimer.stop();
    m_port.write("C\r");
    m_port.waitForBytesWritten(100);
    m_port.close();
    m_openState = osClosed;
}

bool CanAdapterLawicel::transmit(const can_message_t * cmsg)
{
    char str [50];
    slcan_string_from_can_message(str, cmsg);
    m_port.write(str);
    return true;
}

bool CanAdapterLawicel::receive(can_message_t * cmsg)
{
    if(m_openState != osOpen)
        return false;

    m_buffer.append(m_port.readAll());

    int r = m_buffer.indexOf('\r');
    if(r < 0)
        return false;

    // remove that part and get it to line
    QString line = m_buffer.left(r);
    m_buffer.remove(0, r+1);

    return slcan_can_message_from_string(cmsg, line.toStdString().c_str());
}

bool CanAdapterLawicel::isOpen()
{
    return m_openState == osOpen;
}

QWidget * CanAdapterLawicel::getControlWidget(QWidget *parent){
    if(!m_controlWidget)
    {
        m_controlWidget = new SlcanControlWidget(parent);
        connect(m_controlWidget, SIGNAL(openClicked(QString, CanAdapterLawicel::OpenMode, int)), this, SLOT(openClicked(QString, CanAdapterLawicel::OpenMode, int)));
        connect(m_controlWidget, SIGNAL(closeClicked()), this, SLOT(closeClicked()));
        connect(this, SIGNAL(openOperationEnded(bool)), m_controlWidget, SLOT(openOperationEnded(bool)));

    }
    return m_controlWidget;
}

void CanAdapterLawicel::openClicked(QString portName, CanAdapterLawicel::OpenMode mode, int baud){
    m_portName = portName;
    m_canBaudRate = baud;
    m_mode = mode;
    open();
}

void CanAdapterLawicel::closeClicked(){
    close();
}
