#include "CanAdapterLawicel.h"

#include <QThread>
#include <QMessageBox>
#include "lib-slcan/slcan.h"
#include "SlcanControlWidget.h"

CanAdapterLawicel::CanAdapterLawicel(CanHub &canHub)
    : m_portName("COM4"), m_uartBaudRate(1000000), m_openState(osClosed)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(transmit(can_message_t)));
    connect(&m_openTimer, SIGNAL(timeout()), this, SLOT(openTimerTimeout()));
    connect(&m_port, SIGNAL(readyRead()), this, SLOT(readBytesReady()) );
}

CanAdapterLawicel::~CanAdapterLawicel(){
    close();
    delete m_canHandle;
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
        m_openTimer.setInterval(2000);
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
    if(m_openState != osClosed)
    {
        m_openTimer.stop();
        m_port.write("C\r");
        m_port.waitForBytesWritten(100);
        m_port.close();
        m_openState = osClosed;
    }
}

void CanAdapterLawicel::transmit(can_message_t cmsg)
{
    if(m_openState != osOpen)
        return;

    char str [50];
    slcan_string_from_can_message(str, &cmsg);
    m_port.write(str);
}

bool CanAdapterLawicel::receive(can_message_t * cmsg)
{
    if(m_openState != osOpen)
        return false;

    m_buffer.append(m_port.readAll());


    QString line;
    QChar cmd;
    do{
        // read a line from buffer
        int r = m_buffer.indexOf('\r');
        if(r < 0)
            return false;

        // remove that part and get it to line
        line = m_buffer.left(r);
        m_buffer.remove(0, r+1);
        if (line.size() > 0)
            cmd = line.at(0);
    }while(cmd != 't' && cmd != 'T' && cmd != 'r' && cmd != 'R');

    return slcan_can_message_from_string(cmsg, line.toStdString().c_str());
}

void CanAdapterLawicel::readBytesReady()
{
    can_message_t cmsg;
    while(receive(&cmsg)){
        m_canHandle->transmit(cmsg);
    }
}

bool CanAdapterLawicel::isOpen()
{
    return m_openState == osOpen;
}

QWidget * CanAdapterLawicel::getControlWidget(QWidget *parent){
    auto controlWidget = new SlcanControlWidget(parent);
    connect(controlWidget, SIGNAL(openClicked(QString, CanAdapterLawicel::OpenMode, int)), this, SLOT(openClicked(QString, CanAdapterLawicel::OpenMode, int)));
    connect(controlWidget, SIGNAL(closeClicked()), this, SLOT(closeClicked()));
    connect(this, SIGNAL(openOperationEnded(bool)), controlWidget, SLOT(openOperationEnded(bool)));

    return controlWidget;
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
