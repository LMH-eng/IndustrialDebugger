#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include "TcpClient.h"
#include "TcpServer.h"
#include "UdpClient.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    
    // TCP客户端
    void connectToHost(const QString& ip, quint16 port);
    
    // TCP服务端
    void startServer(quint16 port);
    
    // UDP
    void bindUdp(quint16 port);
    
    // 断开连接
    void disconnect();
    
    // 发送数据
    bool sendData(const QByteArray& data);
    
    // 检查连接状态
    bool isConnected() const;

signals:
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void connected();
    void disconnected();

private:
    TcpClient *m_tcpClient;
    TcpServer *m_tcpServer;
    UdpClient *m_udpClient;
    
    enum class Mode {
        None,
        TcpClient,
        TcpServer,
        Udp
    };
    Mode m_currentMode;
};

#endif // NETWORKMANAGER_H
