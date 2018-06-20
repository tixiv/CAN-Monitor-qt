#ifndef CANADAPTERLOOPBACK_H
#define CANADAPTERLOOPBACK_H

#include "CanAdapter.h"

class CanAdapterLoopback : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterLoopback();
    ~CanAdapterLoopback() override;

    bool open() override;
    void close() override;

    bool transmit(const can_message_t * cmsg) override;
    bool receive(can_message_t * cmsg) override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;
private:
    QList<can_message_t> m_messages;
};

#endif // CANADAPTERLOOPBACK_H
