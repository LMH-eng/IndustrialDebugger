#include "LogManager.h"
#include <QFile>
#include <QTextStream>

LogManager::LogManager(QObject *parent)
    : QObject(parent)
{
}

bool LogManager::exportLog(const QString& content, const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << content;
    file.close();
    
    return true;
}

QString LogManager::importLog(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.close();
    
    return content;
}
