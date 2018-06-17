#ifndef HEXSTRINGVALIDATOR_H
#define HEXSTRINGVALIDATOR_H

#include <QValidator>

class HexStringValidator : public QValidator
{
public:
    HexStringValidator(uint32_t minimum, uint32_t maximum);

    void fixup(QString &input) const override;
    QValidator::State validate(QString &input, int &pos) const override;

private:
    uint32_t m_minimum, m_maximum;
    int m_maxLen;
};

#endif // HEXSTRINGVALIDATOR_H
