#ifndef HEXSTRINGVALIDATOR_H
#define HEXSTRINGVALIDATOR_H

#include <QValidator>

class HexStringValidator : public QValidator
{
public:
    HexStringValidator(int minimum, int maximum);

    void fixup(QString &input) const override;
    QValidator::State validate(QString &input, int &pos) const override;

private:
    int m_minimum, m_maximum;
    int m_maxLen;
};

#endif // HEXSTRINGVALIDATOR_H
