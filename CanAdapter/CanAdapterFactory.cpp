#include "CanAdapterFactory.h"

#include "CanAdapterLoopback.h"
#include "CanAdapterLawicel.h"
#include "CanAdapterChina.h"
#include "CanAdapterTesting.h"
#include "CanAdapterTritium.h"
#ifdef BUILD_PCAN
# include "CanAdapterPCAN.h"
#endif

QStringList CanAdapterFactory::getAdapterNames()
{
    return QStringList()
            << "Loopback"
            << "SL-Can / Lawicel"
            << "China"
            << "Tritium CAN Bridge"
            << "Testing"
#ifdef BUILD_PCAN
            << "PCAN"
#endif
            ;
}

CanAdapter * CanAdapterFactory::createAdapter(QString name, CanHub &canHub)
{
    if(name == "Loopback")             return new CanAdapterLoopback(canHub);
    if(name == "SL-Can / Lawicel")     return new CanAdapterLawicel(canHub);
    if(name == "China")                return new CanAdapterChina(canHub);
    if(name == "Tritium CAN Bridge")   return new CanAdapterTritium(canHub);
    if(name == "Testing")              return new CanAdapterTesting(canHub);
#ifdef BUILD_PCAN
    if(name == "PCAN")                 return new CanAdapterPCAN(canHub);
#endif
    return 0;
}
