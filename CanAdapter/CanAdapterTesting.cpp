#include "CanAdapterTesting.h"
#include <QCoreApplication>

CanAdapterTesting::CanAdapterTesting()
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
    return true;
}


static void randomFillCanMessage(can_message_t * cmsg){
    uint16_t * x = (uint16_t*)cmsg;

    for(int i=0; i < sizeof(can_message_t)/2; i++)
        x[i] = qrand();
}

bool CanAdapterTesting::receive(can_message_t * cmsg)
{

    if(m_cnt++ == 10){
        m_cnt = 0;
        return false;
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

QWidget * CanAdapterTesting::getControlWidget(QWidget *parent){
    return 0;
}
