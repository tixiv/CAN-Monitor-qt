#ifndef CANADAPTERFACTORY_H
#define CANADAPTERFACTORY_H

#include <QStringList>

class CanAdapter;

class CanAdapterFactory
{
public:
    static QStringList getAdapterNames();
    static CanAdapter * createAdapter(QString name);
};

#endif // CANADAPTERFACTORY_H
