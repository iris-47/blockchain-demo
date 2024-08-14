#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <QObject>
#include "pages/loggerwidget.h"

// TODO: Logger细化分类
class LoggerManager : public QObject
{
    Q_OBJECT
public:
    static LoggerManager& getInstance();
    // must call this function before using LoggerManager
    void connectWidget(LoggerWidget* widget);
    void disconnetWidget(LoggerWidget* widget);
    void addLog(const QString &message);

signals:
    void logAdded(const QString &message);

private:
    bool m_isConnected = false;

    LoggerManager() = default;
    LoggerManager(const LoggerManager&) = delete;
    LoggerManager& operator=(const LoggerManager&) = delete;
};
#endif // LOGGERMANAGER_H
