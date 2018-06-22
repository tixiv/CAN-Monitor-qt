#ifndef MENUOPENKEEPER_H
#define MENUOPENKEEPER_H

#include <QObject>

class MenuOpenKeeper : public QObject
{
    Q_OBJECT

public:
    MenuOpenKeeper(QObject *parent=0);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MENUOPENKEEPER_H
