#include "CanAdapterTesting.h"
#include "TestingControlWidget.h"
#include <QCoreApplication>

CanAdapterTesting::CanAdapterTesting(CanHub &canHub)
    : PollingCanAdapter(canHub)
{
}

CanAdapterTesting::~CanAdapterTesting(){
}

bool CanAdapterTesting::open()
{
    return true;
}

void CanAdapterTesting::close()
{
}

bool CanAdapterTesting::transmit(const can_message_t * cmsg)
{
    (void) cmsg;
    return true;
}


static void randomFillCanMessage(can_message_t * cmsg)
{
    uint8_t * x = (uint8_t*)cmsg;

    for(unsigned int i=0; i < sizeof(can_message_t); i++)
        x[i] = qrand();
}

bool CanAdapterTesting::receive(can_message_t * cmsg)
{

    if(m_cnt++ == 50){
        m_cnt = 0;
        return false;
    }

    static int num_messages;
    num_messages ++;
    if(num_messages == 100){
        num_messages = 0;
        if(m_timer.isValid()){
            qint64 period = m_timer.restart();

            emit updateMessagesPerSecond(QString::number(period));
        }else{
            m_timer.start();
        }
    }

    randomFillCanMessage(cmsg);
    cmsg->id = cmsg->id % 64;
    cmsg->IDE = 1;
    cmsg->RTR = 0;
    cmsg->dlc = 8;

    return true;
}

bool CanAdapterTesting::isOpen()
{
    return true;
}

QWidget * CanAdapterTesting::getControlWidget(QWidget *parent)
{
    auto controlWidget = new TestingControlWidget(parent);
    connect(this, SIGNAL(updateMessagesPerSecond(QString)), controlWidget, SLOT(updateMessagesPerSecond(QString)));

    return controlWidget;
}
