#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>

class CustomLineEdit : public QLineEdit
{
public:
    void focusInEvent(QFocusEvent *event) override;
    CustomLineEdit();
};

#endif // CUSTOMLINEEDIT_H
