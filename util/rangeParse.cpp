#include "rangeParse.h"

#include <QString>
#include <QRegularExpression>

QSet<int> rangeParse(const QString &str)
{
    auto strs = str.split(',');
    QSet<int> li;

    bool invalid=false;
    foreach (auto str, strs) {
        QRegularExpression re("([0-9a-fA-F]+)[-|:]?([0-9a-fA-F]*)");
        QRegularExpressionMatch match = re.match(str);
        if(match.hasMatch())
        {
            int begin = match.captured(1).toInt(0,16);
            int end = begin;
            if(match.captured(2) != "")
                end = match.captured(2).toInt(0,16);
            if(end < begin)
                invalid = true;
            else
            {
                for(int i=begin; i<=end; i++)
                {
                    li.insert(i);
                }
            }
        }
        else
            invalid = true;
    }

    if(!invalid)
        return li;
    else
        return QSet<int>();
}
