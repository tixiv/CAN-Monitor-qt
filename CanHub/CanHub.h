#ifndef CANHUB_H
#define CANHUB_H

#include <QList>
#include "lib-slcan/can_message.h"

class CanHub;

class CanHandle : public QObject
{
    Q_OBJECT
public:
    void transmit(const can_message_t &cmsg);
    ~CanHandle();

signals:
    void received(can_message_t cmsg);

private:
    friend class CanHub;

    CanHandle(CanHub * hub, int flags)
        : m_hub(hub), m_flags(flags) {}

    void putReceiveMessage(const can_message_t &cmsg);

    CanHub *m_hub;
    int m_flags;
};

class CanHub : public QObject
{
    Q_OBJECT

public:
    CanHub();
    ~CanHub();

    CanHandle * getNewHandle(int flags = 0);

    enum handleFlags
    {
        f_isCanAdapter        = 1,
        f_getAllOtherMessages = 2, // default is from adapter only
        f_getAllMessages      = 4, // including ones produced from self
    };

private:
    friend class CanHandle;
    void transmit(CanHandle * from, const can_message_t &cmsg);

    QList<CanHandle*> m_handles;
};

#endif // CANHUB_H
