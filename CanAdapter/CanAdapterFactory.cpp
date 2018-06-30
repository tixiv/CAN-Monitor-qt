#include "CanAdapterFactory.h"

#include "CanAdapterLoopback.h"
#include "CanAdapterLawicel.h"
#include "CanAdapterTesting.h"

QStringList CanAdapterFactory::getAdapterNames()
{
    return QStringList()
            << "Loopback"
            << "SL-Can / Lawicel"
            << "Testing";
}

CanAdapter * CanAdapterFactory::createAdapter(QString name, CanHub &canHub)
{
    if(name == "Loopback")             return new CanAdapterLoopback(canHub);
    if(name == "SL-Can / Lawicel")     return new CanAdapterLawicel(canHub);
    if(name == "Testing")              return new CanAdapterTesting(canHub);
    return 0;
}
