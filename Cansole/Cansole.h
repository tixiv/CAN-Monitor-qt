#ifndef CANSOLE_H
#define CANSOLE_H

#include <QObject>
#include "CanHub/CanHub.h"

class Cansole : public QObject
{
    Q_OBJECT

public:
    Cansole(CanHub &canHub, int cansoleId);

private slots:
    void messageReceived(can_message_t cmsg);

private:
    CanHandle * m_canHandle;

    int m_cansoleId;
};

#endif // CANSOLE_H
