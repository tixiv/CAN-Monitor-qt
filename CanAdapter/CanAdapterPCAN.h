#ifndef CANADAPTERPCAN_H
#define CANADAPTERPCAN_H

#include <QObject>
#include <QLibrary>
#include <QTimer>
#include "CanAdapter.h"

class CanHub;
class CanHandle;
struct LibraryCalls;

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

    void tickTimerTimeout();
private:
    CanHandle *m_canHandle;

    bool loadPcanLibrary();

    QLibrary m_lib;
    struct LibraryCalls * calls = 0;

    QTimer m_tickTimer;


    bool initialize();
};

#endif // CANADAPTERPCAN_H
