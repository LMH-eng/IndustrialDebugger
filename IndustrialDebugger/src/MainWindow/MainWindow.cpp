#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_networkManager(nullptr)
    , m_serialPortManager(nullptr)
    , m_dataProcessor(new DataProcessor(this))
    , m_logManager(new LogManager(this))
    , m_sendCount(0)
    , m_recvCount(0)
{
    setupUI();
    setupConnections();
    
    // 初始化网络管理器
    m_networkManager = new NetworkManager(this);
    connect(m_networkManager, &NetworkManager::dataReceived, 
            this, &MainWindow::onNetworkDataReceived);
    connect(m_networkManager, &NetworkManager::errorOccurred,
            this, &MainWindow::onNetworkError);
    connect(m_networkManager, &NetworkManager::connected,
            [this]() { m_statusLabel->setText("网络已连接"); });
    connect(m_networkManager, &NetworkManager::disconnected,
            [this]() { m_statusLabel->setText("网络已断开"); });
    
    // 初始化串口管理器
    m_serialPortManager = new SerialPortManager(this);
    connect(m_serialPortManager, &SerialPortManager::dataReceived,
            this, &MainWindow::onSerialPortDataReceived);
    connect(m_serialPortManager, &SerialPortManager::errorOccurred,
            this, &MainWindow::onSerialPortError);
    connect(m_serialPortManager, &SerialPortManager::portOpened,
            [this]() { m_statusLabel->setText("串口已打开"); });
    connect(m_serialPortManager, &SerialPortManager::portClosed,
            [this]() { m_statusLabel->setText("串口已关闭"); });
    
    // 刷新串口列表
    onRefreshSerialPorts();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // 左侧：配置区
    QVBoxLayout *leftLayout = new QVBoxLayout();
    
    // 网络配置组
    m_networkGroup = new QGroupBox("网络配置", this);
    QGridLayout *networkLayout = new QGridLayout(m_networkGroup);
    
    networkLayout->addWidget(new QLabel("模式:"), 0, 0);
    m_networkModeCombo = new QComboBox();
    m_networkModeCombo->addItems({"TCP客户端", "TCP服务端", "UDP"});
    networkLayout->addWidget(m_networkModeCombo, 0, 1);
    
    networkLayout->addWidget(new QLabel("IP地址:"), 1, 0);
    m_ipEdit = new QLineEdit("127.0.0.1");
    networkLayout->addWidget(m_ipEdit, 1, 1);
    
    networkLayout->addWidget(new QLabel("端口:"), 2, 0);
    m_portEdit = new QLineEdit("8888");
    networkLayout->addWidget(m_portEdit, 2, 1);
    
    QHBoxLayout *networkBtnLayout = new QHBoxLayout();
    m_connectBtn = new QPushButton("连接");
    m_disconnectBtn = new QPushButton("断开");
    m_disconnectBtn->setEnabled(false);
    networkBtnLayout->addWidget(m_connectBtn);
    networkBtnLayout->addWidget(m_disconnectBtn);
    networkLayout->addLayout(networkBtnLayout, 3, 0, 1, 2);
    
    leftLayout->addWidget(m_networkGroup);
    
    // 串口配置组
    m_serialGroup = new QGroupBox("串口配置", this);
    QGridLayout *serialLayout = new QGridLayout(m_serialGroup);
    
    serialLayout->addWidget(new QLabel("串口:"), 0, 0);
    m_serialPortCombo = new QComboBox();
    serialLayout->addWidget(m_serialPortCombo, 0, 1);
    m_refreshSerialBtn = new QPushButton("刷新");
    serialLayout->addWidget(m_refreshSerialBtn, 0, 2);
    
    serialLayout->addWidget(new QLabel("波特率:"), 1, 0);
    m_baudRateCombo = new QComboBox();
    m_baudRateCombo->addItems({"9600", "19200", "38400", "57600", "115200"});
    m_baudRateCombo->setCurrentText("115200");
    serialLayout->addWidget(m_baudRateCombo, 1, 1, 1, 2);
    
    serialLayout->addWidget(new QLabel("数据位:"), 2, 0);
    m_dataBitsCombo = new QComboBox();
    m_dataBitsCombo->addItems({"5", "6", "7", "8"});
    m_dataBitsCombo->setCurrentText("8");
    serialLayout->addWidget(m_dataBitsCombo, 2, 1, 1, 2);
    
    serialLayout->addWidget(new QLabel("校验位:"), 3, 0);
    m_parityCombo = new QComboBox();
    m_parityCombo->addItems({"无", "奇校验", "偶校验"});
    serialLayout->addWidget(m_parityCombo, 3, 1, 1, 2);
    
    serialLayout->addWidget(new QLabel("停止位:"), 4, 0);
    m_stopBitsCombo = new QComboBox();
    m_stopBitsCombo->addItems({"1", "1.5", "2"});
    serialLayout->addWidget(m_stopBitsCombo, 4, 1, 1, 2);
    
    QHBoxLayout *serialBtnLayout = new QHBoxLayout();
    m_openSerialBtn = new QPushButton("打开串口");
    m_closeSerialBtn = new QPushButton("关闭串口");
    m_closeSerialBtn->setEnabled(false);
    serialBtnLayout->addWidget(m_openSerialBtn);
    serialBtnLayout->addWidget(m_closeSerialBtn);
    serialLayout->addLayout(serialBtnLayout, 5, 0, 1, 3);
    
    leftLayout->addWidget(m_serialGroup);
    leftLayout->addStretch();
    
    mainLayout->addLayout(leftLayout, 1);
    
    // 右侧：数据收发和日志区
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    // 数据收发组
    m_dataGroup = new QGroupBox("数据收发", this);
    QVBoxLayout *dataLayout = new QVBoxLayout(m_dataGroup);
    
    QHBoxLayout *sendControlLayout = new QHBoxLayout();
    sendControlLayout->addWidget(new QLabel("格式:"));
    m_formatCombo = new QComboBox();
    m_formatCombo->addItems({"ASCII", "Hex"});
    sendControlLayout->addWidget(m_formatCombo);
    sendControlLayout->addStretch();
    dataLayout->addLayout(sendControlLayout);
    
    m_sendEdit = new QTextEdit();
    m_sendEdit->setPlaceholderText("在此输入要发送的数据...");
    m_sendEdit->setMaximumHeight(100);
    dataLayout->addWidget(m_sendEdit);
    
    m_sendBtn = new QPushButton("发送");
    dataLayout->addWidget(m_sendBtn);
    
    rightLayout->addWidget(m_dataGroup);
    
    // 日志显示组
    m_logGroup = new QGroupBox("通信日志", this);
    QVBoxLayout *logLayout = new QVBoxLayout(m_logGroup);
    
    m_logEdit = new QTextEdit();
    m_logEdit->setReadOnly(true);
    logLayout->addWidget(m_logEdit);
    
    QHBoxLayout *logBtnLayout = new QHBoxLayout();
    m_clearLogBtn = new QPushButton("清空日志");
    m_exportLogBtn = new QPushButton("导出日志");
    m_importLogBtn = new QPushButton("导入日志");
    logBtnLayout->addWidget(m_clearLogBtn);
    logBtnLayout->addWidget(m_exportLogBtn);
    logBtnLayout->addWidget(m_importLogBtn);
    logLayout->addLayout(logBtnLayout);
    
    rightLayout->addWidget(m_logGroup);
    
    mainLayout->addLayout(rightLayout, 2);
    
    setCentralWidget(centralWidget);
    
    // 状态栏
    m_statusLabel = new QLabel("就绪");
    m_sendCountLabel = new QLabel("发送: 0 字节");
    m_recvCountLabel = new QLabel("接收: 0 字节");
    statusBar()->addWidget(m_statusLabel);
    statusBar()->addPermanentWidget(m_sendCountLabel);
    statusBar()->addPermanentWidget(m_recvCountLabel);
}

void MainWindow::setupConnections()
{
    // 网络连接
    connect(m_connectBtn, &QPushButton::clicked, this, &MainWindow::onNetworkConnectClicked);
    connect(m_disconnectBtn, &QPushButton::clicked, this, &MainWindow::onNetworkDisconnectClicked);
    
    // 串口连接
    connect(m_openSerialBtn, &QPushButton::clicked, this, &MainWindow::onSerialPortOpenClicked);
    connect(m_closeSerialBtn, &QPushButton::clicked, this, &MainWindow::onSerialPortCloseClicked);
    connect(m_refreshSerialBtn, &QPushButton::clicked, this, &MainWindow::onRefreshSerialPorts);
    
    // 数据发送
    connect(m_sendBtn, &QPushButton::clicked, this, &MainWindow::onSendDataClicked);
    connect(m_formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFormatChanged);
    
    // 日志管理
    connect(m_clearLogBtn, &QPushButton::clicked, this, &MainWindow::onClearLogClicked);
    connect(m_exportLogBtn, &QPushButton::clicked, this, &MainWindow::onExportLogClicked);
    connect(m_importLogBtn, &QPushButton::clicked, this, &MainWindow::onImportLogClicked);
}

void MainWindow::onNetworkConnectClicked()
{
    QString mode = m_networkModeCombo->currentText();
    QString ip = m_ipEdit->text();
    quint16 port = m_portEdit->text().toUShort();
    
    if (mode == "TCP客户端") {
        m_networkManager->connectToHost(ip, port);
    } else if (mode == "TCP服务端") {
        m_networkManager->startServer(port);
    } else if (mode == "UDP") {
        m_networkManager->bindUdp(port);
    }
    
    m_connectBtn->setEnabled(false);
    m_disconnectBtn->setEnabled(true);
}

void MainWindow::onNetworkDisconnectClicked()
{
    m_networkManager->disconnect();
    m_connectBtn->setEnabled(true);
    m_disconnectBtn->setEnabled(false);
}

void MainWindow::onNetworkDataReceived(const QByteArray& data)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString displayData = m_dataProcessor->formatData(data, m_formatCombo->currentText());
    
    m_logEdit->append(QString("[%1] 接收: %2").arg(timestamp).arg(displayData));
    m_recvCount += data.size();
    m_recvCountLabel->setText(QString("接收: %1 字节").arg(m_recvCount));
}

void MainWindow::onNetworkError(const QString& error)
{
    QMessageBox::warning(this, "错误", error);
    m_connectBtn->setEnabled(true);
    m_disconnectBtn->setEnabled(false);
}

void MainWindow::onSerialPortOpenClicked()
{
    QString portName = m_serialPortCombo->currentText();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "错误", "请选择串口");
        return;
    }
    
    qint32 baudRate = m_baudRateCombo->currentText().toInt();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(m_dataBitsCombo->currentText().toInt());
    
    QSerialPort::Parity parity = QSerialPort::NoParity;
    if (m_parityCombo->currentText() == "奇校验") parity = QSerialPort::OddParity;
    else if (m_parityCombo->currentText() == "偶校验") parity = QSerialPort::EvenParity;
    
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    if (m_stopBitsCombo->currentText() == "1.5") stopBits = QSerialPort::OneAndHalfStop;
    else if (m_stopBitsCombo->currentText() == "2") stopBits = QSerialPort::TwoStop;
    
    if (m_serialPortManager->openPort(portName, baudRate, dataBits, parity, stopBits)) {
        m_openSerialBtn->setEnabled(false);
        m_closeSerialBtn->setEnabled(true);
    }
}

void MainWindow::onSerialPortCloseClicked()
{
    m_serialPortManager->closePort();
    m_openSerialBtn->setEnabled(true);
    m_closeSerialBtn->setEnabled(false);
}

void MainWindow::onSerialPortDataReceived(const QByteArray& data)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString displayData = m_dataProcessor->formatData(data, m_formatCombo->currentText());
    
    m_logEdit->append(QString("[%1] 接收: %2").arg(timestamp).arg(displayData));
    m_recvCount += data.size();
    m_recvCountLabel->setText(QString("接收: %1 字节").arg(m_recvCount));
}

void MainWindow::onSerialPortError(const QString& error)
{
    QMessageBox::warning(this, "错误", error);
    m_openSerialBtn->setEnabled(true);
    m_closeSerialBtn->setEnabled(false);
}

void MainWindow::onRefreshSerialPorts()
{
    m_serialPortCombo->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : ports) {
        m_serialPortCombo->addItem(info.portName());
    }
}

void MainWindow::onSendDataClicked()
{
    QString text = m_sendEdit->toPlainText();
    if (text.isEmpty()) return;
    
    QByteArray data = m_dataProcessor->parseData(text, m_formatCombo->currentText());
    
    bool sent = false;
    if (m_networkManager && m_networkManager->isConnected()) {
        sent = m_networkManager->sendData(data);
    } else if (m_serialPortManager && m_serialPortManager->isOpen()) {
        sent = m_serialPortManager->sendData(data);
    } else {
        QMessageBox::warning(this, "错误", "请先连接网络或打开串口");
        return;
    }
    
    if (sent) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        QString displayData = m_dataProcessor->formatData(data, m_formatCombo->currentText());
        m_logEdit->append(QString("[%1] 发送: %2").arg(timestamp).arg(displayData));
        
        m_sendCount += data.size();
        m_sendCountLabel->setText(QString("发送: %1 字节").arg(m_sendCount));
        m_sendEdit->clear();
    }
}

void MainWindow::onClearLogClicked()
{
    m_logEdit->clear();
    m_sendCount = 0;
    m_recvCount = 0;
    m_sendCountLabel->setText("发送: 0 字节");
    m_recvCountLabel->setText("接收: 0 字节");
}

void MainWindow::onExportLogClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出日志", "", "日志文件 (*.txt)");
    if (!fileName.isEmpty()) {
        m_logManager->exportLog(m_logEdit->toPlainText(), fileName);
        QMessageBox::information(this, "成功", "日志导出成功");
    }
}

void MainWindow::onImportLogClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "导入日志", "", "日志文件 (*.txt)");
    if (!fileName.isEmpty()) {
        QString content = m_logManager->importLog(fileName);
        m_logEdit->append(content);
        QMessageBox::information(this, "成功", "日志导入成功");
    }
}

void MainWindow::onFormatChanged(int index)
{
    Q_UNUSED(index);
}
