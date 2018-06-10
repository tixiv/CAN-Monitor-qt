#include "CanAdapterLawicel.h"

#include <QThread>
#include "lawicel.h"

CanAdapterLawicel::CanAdapterLawicel()
    : m_portName("COM4"), m_uartBaudRate(1000000), m_openState(osClosed)
{
    connect(&m_openTimer, &QTimer::timeout, this, &CanAdapterLawicel::openTimerTimeout);
}

/*
void CanAdapterLawicel::configure()
{

}
*/

bool CanAdapterLawicel::open(int baudrate)
{
    if(m_openState != osClosed)
        return false;

    m_canBaudRate = baudrate;

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
    m_port.write("S4\rO\r");
    m_openState = osOpen;
}

void CanAdapterLawicel::close()
{
    m_openTimer.stop();
    m_port.write("C");
    m_port.close();
    m_openState = osClosed;
}

bool CanAdapterLawicel::transmit(const can_message_t * cmsg)
{
    return true;
}

bool CanAdapterLawicel::receive(can_message_t * cmsg)
{
    if(m_openState != osOpen)
        return false;

    m_buffer.append(m_port.readAll());

    // find \r
    int r = m_buffer.indexOf('\r');
    if(r < 0)
        return false;

    // remove that part and get it to line
    QString line = m_buffer.left(r);
    m_buffer.remove(0, r+1);

    return lawicel_can_message_from_string(cmsg, line.toStdString().c_str());
}

bool CanAdapterLawicel::isOpen()
{
    return m_openState == osOpen;
}
