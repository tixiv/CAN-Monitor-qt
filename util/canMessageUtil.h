#ifndef CANMESSAGEUTIL_H
#define CANMESSAGEUTIL_H

#include <QString>
#include "lib-slcan/can_message.h"

QString generateIdString(int id, bool IDE, bool RTR);
QString generateIdString(const can_message_t * cmsg);

QString generateDataString(const can_message_t * cmsg);

#endif // CANMESSAGEUTIL_H
