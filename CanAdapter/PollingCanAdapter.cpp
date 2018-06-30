#include "PollingCanAdapter.h"

PollingCanAdapter::PollingCanAdapter(CanHub &canHub, int pollInterval_ms)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(tickTimerTimeout()));
    m_tickTimer.setInterval(pollInterval_ms);
    m_tickTimer.start();
    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(canAdapterTransmit(can_message_t)));
}

PollingCanAdapter::~PollingCanAdapter()
{
    delete m_canHandle;
}

void PollingCanAdapter::tickTimerTimeout()
{
    can_message_t cmsg;
    while(receive(&cmsg)){
        m_canHandle->transmit(cmsg);
    }
}

void PollingCanAdapter::canAdapterTransmit(can_message_t cmsg)
{
    transmit(&cmsg);
}

