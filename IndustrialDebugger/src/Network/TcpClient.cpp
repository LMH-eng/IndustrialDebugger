#include "TcpClient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &TcpClient::onError);
}

TcpClient::~TcpClient()
{
    if (m_socket) {
        m_socket->disconnectFromHost();
    }
}

void TcpClient::connectToHost(const QString& ip, quint16 port)
{
    m_socket->connectToHost(ip, port);
}

void TcpClient::disconnectFromHost()
{
    m_socket->disconnectFromHost();
}

bool TcpClient::sendData(const QByteArray& data)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        return false;
    }
    
    // 简单的分包协议：4字节长度 + 数据
    quint32 len = data.size();
    QByteArray packet;
    packet.append(reinterpret_cast<char*>(&len), 4);
    packet.append(data);
    
    return m_socket->write(packet) == packet.size();
}

void TcpClient::onReadyRead()
{
    m_buffer.append(m_socket->readAll());
    processBuffer();
}

void TcpClient::onConnected()
{
    emit connected();
}

void TcpClient::onDisconnected()
{
    emit disconnected();
}

void TcpClient::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit errorOccurred(m_socket->errorString());
}

void TcpClient::processBuffer()
{
    // 处理粘包：读取4字节长度，然后读取对应长度的数据
    while (m_buffer.size() >= 4) {
        quint32 len = *reinterpret_cast<const quint32*>(m_buffer.constData());
        
        if (m_buffer.size() < 4 + len) {
            break;  // 数据不完整，等待更多数据
        }
        
        QByteArray data = m_buffer.mid(4, len);
        m_buffer.remove(0, 4 + len);
        emit dataReceived(data);
    }
}
