#include "CanAdapterFactory.h"

#include "CanAdapterLoopback.h"
#include "CanAdapterLawicel.h"

QStringList CanAdapterFactory::getAdapterNames()
{
    return QStringList()
            << "Loopback"
            << "SL-Can / Lawicel";
}

CanAdapter * CanAdapterFactory::createAdapter(QString name)
{
    if(name == "Loopback")             return new CanAdapterLoopback();
    if(name == "SL-Can / Lawicel")     return new CanAdapterLawicel();
    return 0;
}
