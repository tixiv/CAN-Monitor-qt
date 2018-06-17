#include "CustomLineEdit.h"
#include <QTimer>

CustomLineEdit::CustomLineEdit()
{

}

void CustomLineEdit::focusInEvent(QFocusEvent *event)
{
    (void) event;
    QTimer::singleShot(0, [this](){
        selectAll();
    });
}
