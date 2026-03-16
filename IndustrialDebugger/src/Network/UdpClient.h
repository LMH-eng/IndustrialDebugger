#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class UdpClient : public QObject
{
    Q_OBJECT

public:
    explicit UdpClient(QObject *parent = nullptr);
    ~UdpClient();
    
    bool bind(quint16 port);
    void unbind();
    bool sendData(const QByteArray& data);

signals:
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);

private slots:
    void onReadyRead();

private:
    QUdpSocket *m_socket;
    QString m_targetIp;
    quint16 m_targetPort;
};

#endif // UDPCLIENT_H
