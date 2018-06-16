#ifndef CANADAPTER_H
#define CANADAPTER_H

#include <QObject>
#include "lib-slcan/can_message.h"

class CanAdapter : public QObject
{
public:
    virtual bool open(int baudrate) = 0;
    virtual void close() = 0;

    virtual bool transmit(const can_message_t * cmsg) = 0;
    virtual bool receive(can_message_t * cmsg) = 0;

    virtual bool isOpen() = 0;

signals:
    void received();

};

#endif // CANADAPTER_H
