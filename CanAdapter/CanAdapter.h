#ifndef CANADAPTER_H
#define CANADAPTER_H

#include <QObject>
#include "lib-slcan/can_message.h"
#include <QWidget>

class CanAdapter : public QObject
{
    Q_OBJECT

public:
    virtual ~CanAdapter();

    virtual bool open() = 0;
    virtual void close() = 0;

    virtual bool transmit(const can_message_t * cmsg) = 0;
    virtual bool receive(can_message_t * cmsg) = 0;

    virtual bool isOpen() = 0;

    virtual QWidget * getControlWidget(QWidget *parent = 0) = 0;

signals:
    void received();

};

#endif // CANADAPTER_H
