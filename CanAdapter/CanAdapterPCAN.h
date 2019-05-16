#ifndef CANADAPTERPCAN_H
#define CANADAPTERPCAN_H

#include <QObject>
#include "CanAdapter.h"

class CanHub;
class CanHandle;

class CanAdapterPCAN : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterPCAN(CanHub &canHub);
    ~CanAdapterPCAN() override;

    bool open() override;
    void close() override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

private slots:

    void transmit(can_message_t cmsg);

private:
    CanHandle *m_canHandle;

};

#endif // CANADAPTERPCAN_H
