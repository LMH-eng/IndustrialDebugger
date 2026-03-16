#include "DataProcessor.h"

DataProcessor::DataProcessor(QObject *parent)
    : QObject(parent)
{
}

QString DataProcessor::formatData(const QByteArray& data, const QString& format)
{
    if (format == "Hex") {
        return toHexString(data);
    } else {
        return QString::fromUtf8(data);
    }
}

QByteArray DataProcessor::parseData(const QString& text, const QString& format)
{
    if (format == "Hex") {
        return fromHexString(text);
    } else {
        return text.toUtf8();
    }
}

QString DataProcessor::toHexString(const QByteArray& data)
{
    QString result;
    for (int i = 0; i < data.size(); ++i) {
        result += QString("%1 ").arg(static_cast<unsigned char>(data[i]), 2, 16, QChar('0')).toUpper();
    }
    return result.trimmed();
}

QByteArray DataProcessor::fromHexString(const QString& hexString)
{
    QString cleanHex = hexString;
    cleanHex.remove(' ').remove('\n').remove('\r');
    
    QByteArray result;
    for (int i = 0; i < cleanHex.length(); i += 2) {
        QString byteString = cleanHex.mid(i, 2);
        bool ok;
        unsigned char byte = static_cast<unsigned char>(byteString.toUInt(&ok, 16));
        if (ok) {
            result.append(byte);
        }
    }
    return result;
}
