#include "CanAdapterFactory.h"

#include "CanAdapterLoopback.h"
#include "CanAdapterLawicel.h"
#include "CanAdapterChina.h"
#include "CanAdapterTesting.h"
#include "CanAdapterTritium.h"
#include "CanAdapterPCAN.h"


QStringList CanAdapterFactory::getAdapterNames()
{
    return QStringList()
            << "Loopback"
            << "SL-Can / Lawicel"
            << "China"
            << "Tritium CAN Bridge"
            << "Testing"
            << "PCAN";
}

CanAdapter * CanAdapterFactory::createAdapter(QString name, CanHub &canHub)
{
    if(name == "Loopback")             return new CanAdapterLoopback(canHub);
    if(name == "SL-Can / Lawicel")     return new CanAdapterLawicel(canHub);
    if(name == "China")                return new CanAdapterChina(canHub);
    if(name == "Tritium CAN Bridge")   return new CanAdapterTritium(canHub);
    if(name == "Testing")              return new CanAdapterTesting(canHub);
    if(name == "PCAN")                 return new CanAdapterPCAN(canHub);
    return 0;
}
