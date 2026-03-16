#include "MainWindow/MainWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("IndustrialDebugger");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("LiMinghong");
    
    // 加载样式表（可选）
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }
    
    // 创建并显示主窗口
    MainWindow mainWindow;
    mainWindow.setWindowTitle("工业级网络/串口调试助手 v1.0");
    mainWindow.resize(1000, 700);
    mainWindow.show();
    
    return app.exec();
}
