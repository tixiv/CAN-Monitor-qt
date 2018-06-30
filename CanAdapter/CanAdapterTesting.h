#ifndef CANADAPTERTESTING_H
#define CANADAPTERTESTING_H

#include "PollingCanAdapter.h"
#include <QElapsedTimer>

class CanAdapterTesting : public PollingCanAdapter
{
    Q_OBJECT

public:
    CanAdapterTesting(CanHub &canHub);
    ~CanAdapterTesting() override;

    bool open() override;
    void close() override;

    bool transmit(const can_message_t * cmsg) override;
    bool receive(can_message_t * cmsg) override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

signals:
    void updateMessagesPerSecond(QString);
private:

    QElapsedTimer m_timer;
    int m_cnt = 0;


};

#endif // CANADAPTERTESTING_H
