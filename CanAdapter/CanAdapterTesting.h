#ifndef CANADAPTERTESTING_H
#define CANADAPTERTESTING_H

#include "CanAdapter.h"
#include <QElapsedTimer>

class CanAdapterTesting : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterTesting();
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
