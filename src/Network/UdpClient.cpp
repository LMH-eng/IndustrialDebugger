#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpClient::onReadyRead);
}

UdpClient::~UdpClient()
{
    unbind();
}

bool UdpClient::bind(quint16 port)
{
    if (!m_socket->bind(QHostAddress::Any, port)) {
        emit errorOccurred(m_socket->errorString());
        return false;
    }
    return true;
}

void UdpClient::unbind()
{
    m_socket->close();
}

bool UdpClient::sendData(const QByteArray& data)
{
    if (m_targetIp.isEmpty() || m_targetPort == 0) {
        return false;
    }
    
    m_socket->writeDatagram(data, QHostAddress(m_targetIp), m_targetPort);
    return true;
}

void UdpClient::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        
        // 记录发送者信息,用于后续回复
        m_targetIp = sender.toString();
        m_targetPort = senderPort;
        
        emit dataReceived(datagram);
    }
}
