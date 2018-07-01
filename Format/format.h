#ifndef FORMAT_H
#define FORMAT_H

#include <QString>

QString format(const QString &format, const uint8_t data[8], double * firstNumOut);

// int32_t is only used as container for these. How the data is interpreted
// is decided by the given format string. Empty format defaults to int32_t.
QString format(const QString &format, int32_t val);
int32_t parseFormatted(const QString &format, const QString &str);

#endif // FORMAT_H
