#include "canMessageUtil.h"


QString generateIdString(int id, bool IDE, bool RTR)
{
    QString idString;
    if(IDE) idString += "E";
    if(RTR) idString += "R";
    if(IDE || RTR) idString += " ";

    if(IDE)
        idString += QString().sprintf("%08X", id);
    else
        idString += QString().sprintf("%03X", id);

    return idString;
}


QString generateIdString(const can_message_t * cmsg)
{
    return generateIdString(cmsg->id, cmsg->IDE, cmsg->RTR);
}

QString generateDataString(const can_message_t * cmsg)
{
    QString dataString;
    if(!cmsg->RTR) for(int i=0; i<cmsg->dlc; i++){
        dataString += QString().sprintf("%02X ", cmsg->data[i]);
    }
    return dataString;
}
