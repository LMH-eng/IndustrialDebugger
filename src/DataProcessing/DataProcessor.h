#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QByteArray>

class DataProcessor : public QObject
{
    Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr);
    
    // 格式化数据用于显示
    QString formatData(const QByteArray& data, const QString& format);
    
    // 解析用户输入的数据
    QByteArray parseData(const QString& text, const QString& format);
    
    // 字节数组转十六进制字符串
    QString toHexString(const QByteArray& data);
    
    // 十六进制字符串转字节数组
    QByteArray fromHexString(const QString& hexString);
};

#endif // DATAPROCESSOR_H
