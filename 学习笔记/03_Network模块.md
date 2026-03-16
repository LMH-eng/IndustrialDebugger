# Network 模块详解

---

## 1. TCP 通信流程

```
connectToHost() → onConnected() → 连接成功
        ↓
socket 收到数据 → readyRead 信号 → onReadyRead() → processBuffer() → dataReceived 信号
        ↓
MainWindow 收到 dataReceived 信号 → 显示到界面
```

---

## 2. 关键代码解析

### 连接服务器

```cpp
void TcpClient::connectToHost(const QString& ip, quint16 port)
{
    m_socket->connectToHost(ip, port);  // 异步连接，不会阻塞
}
```

### 发送数据

```cpp
bool TcpClient::sendData(const QByteArray& data)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        return false;  // 先检查连接状态
    }

    // 分包协议：4字节长度 + 数据
    quint32 len = data.size();
    QByteArray packet;
    packet.append(reinterpret_cast<char*>(&len), 4);  // 写入长度
    packet.append(data);                               // 写入数据

    return m_socket->write(packet) == packet.size();
}
```

### 接收数据

```cpp
void TcpClient::onReadyRead()
{
    m_buffer.append(m_socket->readAll());  // 追加到缓冲区
    processBuffer();                       // 处理缓冲区
}
```

---

## 3. 信号槽连接（构造函数中）

```cpp
TcpClient::TcpClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    // Qt 5.12 兼容性：使用 error 信号
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &TcpClient::onError);
}
```

---

## 4. 粘包问题（面试高频）

### 问题

TCP 是流式协议，可能两条消息粘在一起收到。

**举例：**
- 发送方发了两条消息："Hello" 和 "World"
- 接收方可能一次收到 "HelloWorld"（粘在一起了）

### 解决方案

定义协议：`4字节长度 + 实际数据`

```cpp
// 发送时
packet.append(4字节长度);
packet.append(数据);

// 接收时
void TcpClient::processBuffer()
{
    while (m_buffer.size() >= 4) {  // 缓冲区至少有4字节
        quint32 len = *reinterpret_cast<const quint32*>(m_buffer.constData());

        if (m_buffer.size() < 4 + len) {
            break;  // 数据不完整，等待更多数据
        }

        QByteArray data = m_buffer.mid(4, len);  // 提取数据
        m_buffer.remove(0, 4 + len);             // 删除已处理的部分
        emit dataReceived(data);                 // 发送信号
    }
}
```

### 面试题

**Q: 为什么会出现粘包？**
- TCP 是流式协议，没有消息边界

**Q: UDP 会粘包吗？**
- 不会，UDP 有消息边界

**Q: 其他解决方法？**
- 分隔符（如 \r\n）
- 固定长度
- 特殊结束符
