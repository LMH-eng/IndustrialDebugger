#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include "Network/NetworkManager.h"
#include "SerialPort/SerialPortManager.h"
#include "DataProcessing/DataProcessor.h"
#include "DataProcessing/LogManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 网络通信槽函数
    void onNetworkConnectClicked();
    void onNetworkDisconnectClicked();
    void onNetworkDataReceived(const QByteArray& data);
    void onNetworkError(const QString& error);
    
    // 串口通信槽函数
    void onSerialPortOpenClicked();
    void onSerialPortCloseClicked();
    void onSerialPortDataReceived(const QByteArray& data);
    void onSerialPortError(const QString& error);
    
    // 数据处理槽函数
    void onSendDataClicked();
    void onClearLogClicked();
    void onExportLogClicked();
    void onImportLogClicked();
    void onFormatChanged(int index);

private:
    void setupUI();
    void setupConnections();
    void updateStatus();
    
    // UI组件 - 网络配置区
    QGroupBox *m_networkGroup;
    QComboBox *m_networkModeCombo;
    QLineEdit *m_ipEdit;
    QLineEdit *m_portEdit;
    QPushButton *m_connectBtn;
    QPushButton *m_disconnectBtn;
    
    // UI组件 - 串口配置区
    QGroupBox *m_serialGroup;
    QComboBox *m_serialPortCombo;
    QComboBox *m_baudRateCombo;
    QComboBox *m_dataBitsCombo;
    QComboBox *m_parityCombo;
    QComboBox *m_stopBitsCombo;
    QPushButton *m_openSerialBtn;
    QPushButton *m_closeSerialBtn;
    QPushButton *m_refreshSerialBtn;
    
    // UI组件 - 数据收发区
    QGroupBox *m_dataGroup;
    QTextEdit *m_sendEdit;
    QPushButton *m_sendBtn;
    QComboBox *m_formatCombo;
    
    // UI组件 - 日志显示区
    QGroupBox *m_logGroup;
    QTextEdit *m_logEdit;
    QPushButton *m_clearLogBtn;
    QPushButton *m_exportLogBtn;
    QPushButton *m_importLogBtn;
    
    // UI组件 - 状态栏
    QLabel *m_statusLabel;
    QLabel *m_sendCountLabel;
    QLabel *m_recvCountLabel;
    
    // 核心模块
    NetworkManager *m_networkManager;
    SerialPortManager *m_serialPortManager;
    DataProcessor *m_dataProcessor;
    LogManager *m_logManager;
    
    // 统计数据
    qint64 m_sendCount;
    qint64 m_recvCount;
};

#endif // MAINWINDOW_H
