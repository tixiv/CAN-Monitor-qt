#include "HexStringValidator.h"
#include <cctype>

HexStringValidator::HexStringValidator(int minimum, int maximum)
    : m_minimum(minimum), m_maximum(maximum), QValidator()
{
    m_maxLen = QString().sprintf("%X", m_maximum).length();
}

void HexStringValidator::fixup(QString &input) const
{
    if(input.length() == 0)
        input = QString().sprintf("%X", m_minimum);

    int num = input.toInt(0, 16);
    if(num > m_maximum)
        input = QString().sprintf("%X", m_maximum);

    while(input.length() < m_maxLen)
        input.prepend('0');
}

QValidator::State HexStringValidator::validate(QString &input, int &pos) const
{
    {
        QString temp;
        foreach(QChar ch, input) {
            if(std::isxdigit(ch.toLatin1())) {
                temp += ch.toUpper();
            }
        }
        input = temp;
    }

    if(input.length() == 0)
        return QValidator::Intermediate;


    if(input.length() > m_maxLen){
        input.remove(0,input.length()-m_maxLen);
    }

    int num = input.toInt(0, 16);
    if(num > m_maximum)
        return QValidator::Intermediate;


    if(input.length() == m_maxLen)
        return QValidator::Acceptable;
    else
        return QValidator::Intermediate;
}
