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

CanAdapter * CanAdapterFactory::createAdapter(QString name)
{
    if(name == "Loopback")             return new CanAdapterLoopback();
    if(name == "SL-Can / Lawicel")     return new CanAdapterLawicel();
    if(name == "Testing")              return new CanAdapterTesting();
    return 0;
}
