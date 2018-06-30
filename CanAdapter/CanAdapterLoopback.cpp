#include "CanAdapterLoopback.h"

CanAdapterLoopback::CanAdapterLoopback(CanHub &canHub)
    : PollingCanAdapter(canHub)
{

}
CanAdapterLoopback::~CanAdapterLoopback(){
}

bool CanAdapterLoopback::open()
{
    return true;
}

void CanAdapterLoopback::close()
{
}

bool CanAdapterLoopback::transmit(const can_message_t * cmsg)
{
    m_messages.append(*cmsg);
    return true;
}

bool CanAdapterLoopback::receive(can_message_t * cmsg)
{
    if(m_messages.size()){
        memcpy(cmsg, &m_messages.at(0), sizeof(can_message_t));
        m_messages.removeAt(0);
        return true;
    }
    else
        return false;
}

bool CanAdapterLoopback::isOpen()
{
    return true;
}

QWidget * CanAdapterLoopback::getControlWidget(QWidget *parent){
    (void) parent;
    return 0;
}
