#include "MessageTreeNode.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QRegularExpression>

MessageTreeNode::MessageTreeNode()
    :TreeNode(), IDE(false), RTR(false), id(0)
{
}

MessageTreeNode::MessageTreeNode(const QString &name, int id, bool IDE, bool RTR)
    :TreeNode(), m_name(name), IDE(IDE), RTR(RTR), id(id)
{
}

MessageTreeNode::MessageTreeNode(const can_message_t * cmsg)
    :IDE(cmsg->IDE), RTR(cmsg->RTR), id(cmsg->id)
{
    initIdString();

    m_name = "ID " + m_idString;

    update(cmsg);
}

void MessageTreeNode::initIdString()
{
    if(IDE) m_idString += "E";
    if(RTR) m_idString += "R";
    if(IDE || RTR) m_idString += " ";

    if(IDE)
        m_idString += QString().sprintf("%08X", id);
    else
        m_idString += QString().sprintf("%03X", id);
}


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

static int formatOne(QString &output, const QString &format, const uint8_t data[8], int byteIndex, double * numOut)
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

static QString format(const QString &format, const uint8_t data[8], double * firstNumOut)
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

void MessageTreeNode::updateDataDecoded()
{
    m_firstDecodedNum = 0.0;
    m_dataDecodedString = format(m_formatString, data, &m_firstDecodedNum);
}

void MessageTreeNode::update(const can_message_t * cmsg)
{
    if(m_timer.isValid()){
        m_period = m_timer.restart();
        m_periodString = QString::number(m_period);
    }else{
        m_timer.start();
    }
    dlc = cmsg->dlc;
    memcpy(data, cmsg->data, 8);

    m_dlcString = QString::number(dlc);
    m_dataString = "";

    if(!RTR) for(int i=0; i<dlc; i++){
        m_dataString += QString().sprintf("%02X ", data[i]);
    }

    m_count++;
    m_countString = QString::number(m_count);
    updateDataDecoded();
}

QVariant MessageTreeNode::getData(dataFunction df, int role) const
{
    if(role == Qt::UserRole){ // sort role
        switch(df){
        case dfID:          return (IDE << 30) | (RTR << 29) | id;
        case dfCount:       return m_count;
        case dfPeriod:      return m_period;
        case dfDataDecoded: return m_firstDecodedNum;
        default: break;
        }
    }

    switch(df){
    case dfName:        return m_name;
    case dfID:          return m_idString;
    case dfDLC:         return m_dlcString;
    case dfCount:       return m_countString;
    case dfPeriod:      return m_periodString;
    case dfRawData:     return m_dataString;
    case dfDataDecoded: return m_dataDecodedString;
    case dfFormat:      return m_formatString;
    }

    return QVariant();
}

bool MessageTreeNode::setData(dataFunction df, const QVariant &value)
{
    if(df == dfName){
        m_name = value.toString();
        return true;
    }else if(df == dfFormat){
        m_formatString = value.toString();
        updateDataDecoded();
        return true;
    }else{
        return false;
    }
}

void MessageTreeNode::writeDataToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("name", m_name);
    writer.writeAttribute("id", QString().sprintf("%X", id));
    writer.writeAttribute("IDE", IDE ? "true":"false");
    writer.writeAttribute("RTR", RTR ? "true":"false");
    writer.writeAttribute("format", m_formatString);
}

void MessageTreeNode::readDataFromXml(QXmlStreamReader &reader)
{
    m_name = reader.attributes().value("name").toString();
    id = reader.attributes().value("id").toString().toUInt(0,16);
    IDE = reader.attributes().value("IDE").toString() == "true";
    RTR = reader.attributes().value("RTR").toString() == "true";
    m_formatString = reader.attributes().value("format").toString();
    initIdString();
}
