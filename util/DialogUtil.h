#ifndef DIALOGUTIL_H
#define DIALOGUTIL_H

#include <QLineEdit>

class DialogUtil
{
public:

    template<typename T> static bool checkIntLineEdit(QLineEdit *le, T *result, int radix = 10)
    {
        bool ok;
        int r = le->text().toInt(&ok,radix);
        if(!ok){
            le->setFocus();
            le->selectAll();
        } else {
            *result = r;
        }
        return ok;
    }
};

#endif // DIALOGUTIL_H
