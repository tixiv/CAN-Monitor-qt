#ifndef CANADAPTERFACTORY_H
#define CANADAPTERFACTORY_H

#include <QStringList>

class CanAdapter;
class CanHub;

class CanAdapterFactory
{
public:
    static QStringList getAdapterNames();
    static CanAdapter * createAdapter(QString name,  CanHub &canHub);
};

#endif // CANADAPTERFACTORY_H
