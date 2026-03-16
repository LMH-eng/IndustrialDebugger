# MainWindow 模块详解

---

## 整体结构

**MainWindow = UI界面 + 模块协调 + 信号槽连接**

```
构造函数做三件事：
1. setupUI()        → 创建所有界面组件
2. setupConnections() → 连接信号槽
3. 初始化模块      → NetworkManager、SerialPortManager
```

---

## UI 布局结构

```
┌─────────────────────────────────────────────┐
│  状态栏：就绪 | 发送: 0 字节 | 接收: 0 字节   │
├──────────────┬──────────────────────────────┤
│  网络配置     │     数据收发                  │
│  ├─ 模式选择  │     ├─ 格式选择 (ASCII/Hex)   │
│  ├─ IP地址    │     ├─ 发送输入框             │
│  ├─ 端口      │     └─ 发送按钮               │
│  ├─ 连接按钮   ├──────────────────────────────┤
│  └─ 断开按钮   │     通信日志                  │
├──────────────┤     ├─ 日志显示区              │
│  串口配置     │     ├─ 清空日志               │
│  ├─ 串口选择  │     ├─ 导出日志               │
│  ├─ 波特率    │     └─ 导入日志               │
│  ├─ 数据位    │                              │
│  ├─ 校验位    │                              │
│  ├─ 停止位    │                              │
│  ├─ 打开串口  │                              │
│  └─ 关闭串口  │                              │
└──────────────┴──────────────────────────────┘
```

---

## 第1块：头文件引入

```cpp
#include "MainWindow.h"        // 自己的头文件
#include <QVBoxLayout>         // 垂直布局
#include <QHBoxLayout>         // 水平布局
#include <QGridLayout>         // 网格布局
#include <QSerialPortInfo>     // 串口信息
#include <QMessageBox>         // 消息对话框
#include <QFileDialog>         // 文件对话框
#include <QDateTime>           // 日期时间
#include <QStatusBar>          // 状态栏
```

---

## 第2块：构造函数

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_networkManager(nullptr)      // 先置空
    , m_serialPortManager(nullptr)
    , m_dataProcessor(new DataProcessor(this))  // 立即创建
    , m_logManager(new LogManager(this))
    , m_sendCount(0)
    , m_recvCount(0)
{
    setupUI();           // 第一步：创建界面
    setupConnections();  // 第二步：连接信号槽

    // 第三步：初始化网络管理器
    m_networkManager = new NetworkManager(this);
    connect(m_networkManager, &NetworkManager::dataReceived,
            this, &MainWindow::onNetworkDataReceived);

    // 第四步：初始化串口管理器
    m_serialPortManager = new SerialPortManager(this);

    // 第五步：刷新串口列表
    onRefreshSerialPorts();
}
```

**为什么网络和串口管理器在构造函数体内创建？**
- 因为创建后要立即 connect，放在一起更清晰

---

## 第3块：setupUI() - 界面创建

### （1）创建中央部件

```cpp
QWidget *centralWidget = new QWidget(this);
QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
// 主布局是水平的：左边配置区 | 右边数据区
```

### （2）网络配置组

```cpp
m_networkGroup = new QGroupBox("网络配置", this);
QGridLayout *networkLayout = new QGridLayout(m_networkGroup);

// 第0行：模式选择
networkLayout->addWidget(new QLabel("模式:"), 0, 0);
m_networkModeCombo = new QComboBox();
m_networkModeCombo->addItems({"TCP客户端", "TCP服务端", "UDP"});

// 第1-2行：IP和端口
m_ipEdit = new QLineEdit("127.0.0.1");
m_portEdit = new QLineEdit("8888");

// 第3行：按钮
m_connectBtn = new QPushButton("连接");
m_disconnectBtn = new QPushButton("断开");
m_disconnectBtn->setEnabled(false);  // 初始禁用
```

**GridLayout 坐标图：**
```
   列0        列1
行0 [模式:]   [下拉框]
行1 [IP地址:] [输入框]
行2 [端口:]   [输入框]
行3 [连接按钮] [断开按钮]
```

---

## 第4块：setupConnections() - 信号槽连接

```cpp
void MainWindow::setupConnections()
{
    // 网络按钮
    connect(m_connectBtn, &QPushButton::clicked,
            this, &MainWindow::onNetworkConnectClicked);
    connect(m_disconnectBtn, &QPushButton::clicked,
            this, &MainWindow::onNetworkDisconnectClicked);

    // 串口按钮
    connect(m_openSerialBtn, &QPushButton::clicked,
            this, &MainWindow::onSerialPortOpenClicked);
    connect(m_closeSerialBtn, &QPushButton::clicked,
            this, &MainWindow::onSerialPortCloseClicked);

    // 数据发送
    connect(m_sendBtn, &QPushButton::clicked,
            this, &MainWindow::onSendDataClicked);

    // 日志管理
    connect(m_clearLogBtn, &QPushButton::clicked,
            this, &MainWindow::onClearLogClicked);
}
```

**规律：按钮 clicked 信号 → 对应的槽函数**

---

## 第5块：网络连接槽函数

```cpp
void MainWindow::onNetworkConnectClicked()
{
    QString mode = m_networkModeCombo->currentText();  // 获取模式
    QString ip = m_ipEdit->text();                      // 获取IP
    quint16 port = m_portEdit->text().toUShort();       // 获取端口

    if (mode == "TCP客户端") {
        m_networkManager->connectToHost(ip, port);
    } else if (mode == "TCP服务端") {
        m_networkManager->startServer(port);
    } else if (mode == "UDP") {
        m_networkManager->bindUdp(port);
    }

    m_connectBtn->setEnabled(false);     // 禁用连接按钮
    m_disconnectBtn->setEnabled(true);   // 启用断开按钮
}
```

**按钮状态切换：**
```
初始状态：[连接]可用  [断开]禁用
点击连接：[连接]禁用  [断开]可用
点击断开：[连接]可用  [断开]禁用
```

---

## 第6块：数据接收槽函数

```cpp
void MainWindow::onNetworkDataReceived(const QByteArray& data)
{
    // 1. 获取当前时间戳
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    // 2. 格式化数据
    QString displayData = m_dataProcessor->formatData(data, m_formatCombo->currentText());

    // 3. 追加到日志
    m_logEdit->append(QString("[%1] 接收: %2").arg(timestamp).arg(displayData));

    // 4. 更新计数
    m_recvCount += data.size();
    m_recvCountLabel->setText(QString("接收: %1 字节").arg(m_recvCount));
}
```

**数据流转：**
```
网络收到数据 → NetworkManager 发出 dataReceived 信号
    ↓
MainWindow::onNetworkDataReceived 被调用
    ↓
添加时间戳 → 格式化数据 → 显示到日志 → 更新统计
```

---

## 第7块：数据发送槽函数

```cpp
void MainWindow::onSendDataClicked()
{
    // 1. 获取输入
    QString text = m_sendEdit->toPlainText();
    if (text.isEmpty()) return;

    // 2. 解析为字节
    QByteArray data = m_dataProcessor->parseData(text, m_formatCombo->currentText());

    // 3. 判断发送通道
    bool sent = false;
    if (m_networkManager && m_networkManager->isConnected()) {
        sent = m_networkManager->sendData(data);      // 网络发送
    } else if (m_serialPortManager && m_serialPortManager->isOpen()) {
        sent = m_serialPortManager->sendData(data);   // 串口发送
    } else {
        QMessageBox::warning(this, "错误", "请先连接网络或打开串口");
        return;
    }

    // 4. 更新日志和统计
    if (sent) {
        m_logEdit->append(...);
        m_sendCount += data.size();
        m_sendEdit->clear();  // 清空输入框
    }
}
```

---

## 第8块：串口打开槽函数

```cpp
void MainWindow::onSerialPortOpenClicked()
{
    // 获取串口名
    QString portName = m_serialPortCombo->currentText();

    // 获取配置参数
    qint32 baudRate = m_baudRateCombo->currentText().toInt();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(
        m_dataBitsCombo->currentText().toInt());

    // 校验位转换
    QSerialPort::Parity parity = QSerialPort::NoParity;
    if (m_parityCombo->currentText() == "奇校验") parity = QSerialPort::OddParity;
    else if (m_parityCombo->currentText() == "偶校验") parity = QSerialPort::EvenParity;

    // 打开串口
    if (m_serialPortManager->openPort(portName, baudRate, dataBits, parity, stopBits)) {
        m_openSerialBtn->setEnabled(false);
        m_closeSerialBtn->setEnabled(true);
    }
}
```

**串口参数：**
| 参数 | 作用 | 常用值 |
|------|------|--------|
| 波特率 | 数据传输速度 | 9600, 115200 |
| 数据位 | 每帧数据位数 | 8 |
| 校验位 | 错误检测 | 无/奇/偶 |
| 停止位 | 帧结束标志 | 1 |

---

## 整体流程回顾

```
程序启动 → main() 创建 MainWindow
    ↓
构造函数：setupUI() → setupConnections() → 初始化模块
    ↓
用户操作 → 信号触发 → 槽函数执行
    ↓
数据收发 → 日志显示 → 统计更新
```
