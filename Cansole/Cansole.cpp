#include "Cansole.h"

Cansole::Cansole(CanHub &canHub, int cansoleId)
    : m_cansoleId(cansoleId)
{
    m_canHandle = canHub.getNewHandle();
    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(messageReceived(can_message_t)));

}

void Cansole::messageReceived(can_message_t cmsg)
{

}
