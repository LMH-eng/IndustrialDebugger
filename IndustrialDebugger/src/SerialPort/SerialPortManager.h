#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();
    
    bool openPort(const QString& portName, qint32 baudRate, 
                  QSerialPort::DataBits dataBits, 
                  QSerialPort::Parity parity,
                  QSerialPort::StopBits stopBits);
    void closePort();
    bool sendData(const QByteArray& data);
    bool isOpen() const;

signals:
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void portOpened();
    void portClosed();

private slots:
    void onReadyRead();
    void onError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
};

#endif // SERIALPORTMANAGER_H
