
#include "format.h"
#include <QStringList>
#include <QRegularExpression>



static uint32_t readUnsigned(const uint8_t * p, int numBytes, bool bigEndian )
{
    uint32_t val = 0;
    if(bigEndian)
        for(int i=0; i<numBytes; i++)
        {
            val <<= 8;
            val |= p[i];
        }
    else
        for(int i=numBytes-1; i>=0; i--)
        {
            val <<= 8;
            val |= p[i];
        }
    return val;
}

static int32_t readSigned(const uint8_t * p, int numBytes, bool bigEndian)
{
    int32_t val = (int32_t)readUnsigned(p,numBytes,bigEndian);
    if((val >> ((numBytes-1)*8)) & 0x80)
        val |= 0xFFFFFF00 << ((numBytes-1)*8); // sign extend if negative
    return val;
}

static float readFloat(const uint8_t * p)
{
    union{
        uint32_t w;
        float f;
    }u;
    u.w = readUnsigned(p, 4, false);
    return u.f;
}

// Example of format strings:
// "u16 0.1:%0.1f"
// "u8"
// "s32 0.01:0.2 Volt"
// "f 10:Wert 0 ist %0.2 Volt"

static int formatOne(QString &output, const QString &format, const uint8_t * data, int byteIndex, double * numOut)
{
    QStringList parts = format.split(':');

    double num = 0.0;
    int newByteIndex = byteIndex;
    QString multiplierPart;
    int numDigits = 0;

    {
        // Parse the number specifier part
        // after this block num should hold our number
        // and multiplierPart the (optional) multiplier string
        QRegularExpression re("(u|U|s|S)(8|16|32)(.*)");
        QRegularExpressionMatch match = re.match(parts[0]);
        if (match.hasMatch()) {
            int numBytes = match.captured(2).toInt() / 8;
            newByteIndex += numBytes;
            if(newByteIndex > 8)
                return 8;
            if(match.captured(1) == "u")
                num = readUnsigned(&data[byteIndex], numBytes, false);
            else if(match.captured(1) == "U")
                num = readUnsigned(&data[byteIndex], numBytes, true);
            else if(match.captured(1) == "s")
                num = readSigned(&data[byteIndex], numBytes, false);
            else if(match.captured(1) == "S")
                num = readSigned(&data[byteIndex], numBytes, true);

            multiplierPart = match.captured(3);
        }else{
            QRegularExpression re("f(.*)");
            QRegularExpressionMatch match = re.match(parts[0]);
            if(match.hasMatch()){
                newByteIndex += 4;
                if(newByteIndex > 8)
                    return 8;
                num = readFloat(&data[byteIndex]);
                multiplierPart = match.captured(1);
                numDigits = 2; // 2 default digits for float
            }else{
                return newByteIndex;
            }
        }
    }

    {
        // Handle the multiplier
        bool ok = false;
        double multiplier = multiplierPart.toDouble(&ok);
        if(ok){
            num *= multiplier; // multiply the number

            while(multiplier < 1.0){
                numDigits++;  // calculate an appropriate number of default digits
                multiplier *= 10.0;
            }
        }
    }

    QString post;
    if(parts.size() > 1){
        // Handle format string if given
        QRegularExpression re("(.*?)%?\\d?\\.(\\d+)f?(.*)");
        QRegularExpressionMatch match = re.match(parts[1]);
        if(match.hasMatch()){
            output += match.captured(1);
            numDigits = match.captured(2).toInt();
            post = match.captured(3);
        }else{
            output += parts[1];
        }
    }

    if(numOut)
        *numOut = num;

    output += QString().setNum(num, 'f', numDigits);
    output += post;
    return newByteIndex;
}

QString format(const QString &format, const uint8_t data[8], double * firstNumOut)
{
    QString str;
    QStringList formats = format.split(',');
    int i = 0;
    bool first = true;
    foreach (auto f, formats) {
        if(!first)
            str += ", ";
        i = formatOne(str, f, data, i, first ? firstNumOut : 0);
        first = false;
        if(i==8) break;
    }
    return str;
}
