#include "NetworkManager.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_tcpClient(nullptr)
    , m_tcpServer(nullptr)
    , m_udpClient(nullptr)
    , m_currentMode(Mode::None)
{
}

NetworkManager::~NetworkManager()
{
    disconnect();
}

void NetworkManager::connectToHost(const QString& ip, quint16 port)
{
    disconnect();
    m_tcpClient = new TcpClient(this);
    connect(m_tcpClient, &TcpClient::dataReceived, this, &NetworkManager::dataReceived);
    connect(m_tcpClient, &TcpClient::errorOccurred, this, &NetworkManager::errorOccurred);
    connect(m_tcpClient, &TcpClient::connected, this, &NetworkManager::connected);
    connect(m_tcpClient, &TcpClient::disconnected, this, &NetworkManager::disconnected);
    
    m_tcpClient->connectToHost(ip, port);
    m_currentMode = Mode::TcpClient;
}

void NetworkManager::startServer(quint16 port)
{
    disconnect();
    m_tcpServer = new TcpServer(this);
    connect(m_tcpServer, &TcpServer::dataReceived, this, &NetworkManager::dataReceived);
    connect(m_tcpServer, &TcpServer::errorOccurred, this, &NetworkManager::errorOccurred);
    
    if (m_tcpServer->start(port)) {
        emit connected();
    }
    m_currentMode = Mode::TcpServer;
}

void NetworkManager::bindUdp(quint16 port)
{
    disconnect();
    m_udpClient = new UdpClient(this);
    connect(m_udpClient, &UdpClient::dataReceived, this, &NetworkManager::dataReceived);
    connect(m_udpClient, &UdpClient::errorOccurred, this, &NetworkManager::errorOccurred);
    
    if (m_udpClient->bind(port)) {
        emit connected();
    }
    m_currentMode = Mode::Udp;
}

void NetworkManager::disconnect()
{
    if (m_tcpClient) {
        m_tcpClient->disconnectFromHost();
        m_tcpClient->deleteLater();
        m_tcpClient = nullptr;
    }
    
    if (m_tcpServer) {
        m_tcpServer->stop();
        m_tcpServer->deleteLater();
        m_tcpServer = nullptr;
    }
    
    if (m_udpClient) {
        m_udpClient->unbind();
        m_udpClient->deleteLater();
        m_udpClient = nullptr;
    }
    
    m_currentMode = Mode::None;
    emit disconnected();
}

bool NetworkManager::sendData(const QByteArray& data)
{
    switch (m_currentMode) {
    case Mode::TcpClient:
        return m_tcpClient ? m_tcpClient->sendData(data) : false;
    case Mode::TcpServer:
        return m_tcpServer ? m_tcpServer->sendData(data) : false;
    case Mode::Udp:
        return m_udpClient ? m_udpClient->sendData(data) : false;
    default:
        return false;
    }
}

bool NetworkManager::isConnected() const
{
    return m_currentMode != Mode::None;
}
