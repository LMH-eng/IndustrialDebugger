#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();
    
    void connectToHost(const QString& ip, quint16 port);
    void disconnectFromHost();
    bool sendData(const QByteArray& data);

signals:
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void connected();
    void disconnected();

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    void processBuffer();
};

#endif // TCPCLIENT_H
