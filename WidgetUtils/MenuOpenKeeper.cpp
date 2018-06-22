#include "MenuOpenKeeper.h"
#include <QEvent>
#include <QMenu>

MenuOpenKeeper::MenuOpenKeeper(QObject *parent)
    : QObject(parent)
{
}

bool MenuOpenKeeper::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMenu *menu = dynamic_cast<QMenu *>(obj);
        if(menu && menu->activeAction())
            menu->activeAction()->trigger();
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
