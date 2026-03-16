#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
}

TcpServer::~TcpServer()
{
    stop();
}

bool TcpServer::start(quint16 port)
{
    if (!m_server->listen(QHostAddress::Any, port)) {
        emit errorOccurred(m_server->errorString());
        return false;
    }
    return true;
}

void TcpServer::stop()
{
    for (QTcpSocket* client : m_clients) {
        client->disconnectFromHost();
        client->deleteLater();
    }
    m_clients.clear();
    m_server->close();
}

bool TcpServer::sendData(const QByteArray& data)
{
    if (m_clients.isEmpty()) {
        return false;
    }
    
    // 简单的分包协议：4字节长度 + 数据
    quint32 len = data.size();
    QByteArray packet;
    packet.append(reinterpret_cast<char*>(&len), 4);
    packet.append(data);
    
    // 发送给所有客户端
    for (QTcpSocket* client : m_clients) {
        client->write(packet);
    }
    return true;
}

void TcpServer::onNewConnection()
{
    QTcpSocket* client = m_server->nextPendingConnection();
    m_clients.append(client);
    
    connect(client, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &TcpServer::onClientDisconnected);
}

void TcpServer::onReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;
    
    QByteArray data = client->readAll();
    emit dataReceived(data);
}

void TcpServer::onClientDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;
    
    m_clients.removeOne(client);
    client->deleteLater();
}
