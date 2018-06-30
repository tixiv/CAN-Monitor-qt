#ifndef POLLINGCANADAPTER_H
#define POLLINGCANADAPTER_H

#include "CanAdapter.h"
#include "CanHub/CanHub.h"
#include <QTimer>

class PollingCanAdapter : public CanAdapter
{
    Q_OBJECT

public:
    PollingCanAdapter(CanHub &canHub, int pollInterval_ms=20);
    virtual ~PollingCanAdapter();

protected:
    virtual bool transmit(const can_message_t * cmsg) = 0;
    virtual bool receive(can_message_t * cmsg) = 0;

private slots:
    void canAdapterTransmit(can_message_t cmsg);
    void tickTimerTimeout();
private:
    CanHandle * m_canHandle = 0;
    QTimer m_tickTimer;


};

#endif // POLLINGCANADAPTER_H
