#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QString>

class LogManager : public QObject
{
    Q_OBJECT

public:
    explicit LogManager(QObject *parent = nullptr);
    
    // 导出日志到文件
    bool exportLog(const QString& content, const QString& fileName);
    
    // 从文件导入日志
    QString importLog(const QString& fileName);
};

#endif // LOGMANAGER_H
