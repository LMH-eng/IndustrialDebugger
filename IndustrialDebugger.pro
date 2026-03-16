QT += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = IndustrialDebugger
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/MainWindow/MainWindow.cpp \
    src/Network/NetworkManager.cpp \
    src/Network/TcpClient.cpp \
    src/Network/TcpServer.cpp \
    src/Network/UdpClient.cpp \
    src/SerialPort/SerialPortManager.cpp \
    src/DataProcessing/DataProcessor.cpp \
    src/DataProcessing/LogManager.cpp

HEADERS += \
    src/MainWindow/MainWindow.h \
    src/Network/NetworkManager.h \
    src/Network/TcpClient.h \
    src/Network/TcpServer.h \
    src/Network/UdpClient.h \
    src/SerialPort/SerialPortManager.h \
    src/DataProcessing/DataProcessor.h \
    src/DataProcessing/LogManager.h

# 设置输出目录
DESTDIR = $$PWD/bin

# Windows 特定配置
win32 {
    # 控制台输出（方便调试）
    CONFIG += console
}

# 安装路径
target.path = $$PWD/bin
INSTALLS += target
