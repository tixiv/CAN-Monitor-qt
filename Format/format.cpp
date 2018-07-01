
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

struct ParsedFormat
{
    int numBytes;
    bool isFloat;
    bool isSigned;
    bool isBigEndian;
    int numDigits;
    double multiplier;
    QString pre;
    QString post;
};

static bool parseOne(ParsedFormat &pf, const QString &format)
{
    pf.numBytes = 0;
    pf.isFloat = false;
    pf.isSigned = false;
    pf.isBigEndian = false;
    pf.numDigits = 0;
    pf.multiplier = 1.0;

    QStringList parts = format.split(':');

    QString multiplierPart;

    {
        // Parse the number specifier part
        // after this block num should hold our number
        // and multiplierPart the (optional) multiplier string
        QRegularExpression re("(u|U|s|S)(8|16|32)(.*)");
        QRegularExpressionMatch match = re.match(parts[0]);
        if (match.hasMatch()) {
            pf.numBytes = match.captured(2).toInt() / 8;

            if(match.captured(1) == "u"){

            }else if(match.captured(1) == "U"){
                pf.isBigEndian = true;
            }else if(match.captured(1) == "s"){
                pf.isSigned = true;
            }else if(match.captured(1) == "S"){
                pf.isSigned = true; pf.isBigEndian = true;
            }

            multiplierPart = match.captured(3);
        }else{
            QRegularExpression re("f(.*)");
            QRegularExpressionMatch match = re.match(parts[0]);
            if(match.hasMatch()){
                pf.numBytes = 4;
                multiplierPart = match.captured(1);
                pf.numDigits = 2; // 2 default digits for float
            }else{
                return false;
            }
        }
    }

    {
        // Handle the multiplier
        bool ok = false;
        double multiplier = multiplierPart.toDouble(&ok);
        if(ok){
            pf.multiplier = multiplier;

            while(multiplier < 1.0){
                pf.numDigits++;  // calculate an appropriate number of default digits
                multiplier *= 10.0;
            }
        }
    }

    if(parts.size() > 1){
        // Handle format string if given
        QRegularExpression re("(.*?)%?\\d?\\.(\\d+)f?(.*)");
        QRegularExpressionMatch match = re.match(parts[1]);
        if(match.hasMatch()){
            pf.pre = match.captured(1);
            pf.numDigits = match.captured(2).toInt();
            pf.post = match.captured(3);
        }else{
            pf.post = parts[1];
        }
    }

    return true;
}

static int formatOne(QString &output, const QString &format, const uint8_t * data, int byteIndex, double * numOut)
{

    double num = 0.0;
    int newByteIndex = byteIndex;

    ParsedFormat pf;
    if(parseOne(pf, format))
    {
        newByteIndex += pf.numBytes;
        if(newByteIndex > 8){
            newByteIndex = 8;
            goto exit;
        }

        if(pf.isFloat){
            num = readFloat(&data[byteIndex]);
        }else{
            if(pf.isSigned)
            {
                if(!pf.isBigEndian)
                    num = readSigned(&data[byteIndex], pf.numBytes, false);
                else
                    num = readSigned(&data[byteIndex], pf.numBytes, true);
            }
            else
            {
                if(!pf.isBigEndian)
                    num = readUnsigned(&data[byteIndex], pf.numBytes, false);
                else
                    num = readUnsigned(&data[byteIndex], pf.numBytes, true);
            }
        }

        num *= pf.multiplier; // multiply the number

        output += pf.pre;
        output += QString().setNum(num, 'f', pf.numDigits);
        output += pf.post;
    }

    exit:
    if(numOut)
        *numOut = num;

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

QString format(const QString &format, int32_t val)
{
    QString output;
    const uint8_t * data = (const uint8_t *) &val;
    if(format == "")
        formatOne(output, "s32", data, 0, 0);
    else
        formatOne(output, format, data, 0, 0);

    return output;
}

int32_t parseFormatted(const QString &format, const QString &str)
{

}
