#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <QObject>
#include <QVector>
#include <QTabWidget>
#include "pages/loggerwidget.h"

// TODO: Logger细化分类
class LoggerManager : public QObject
{
    Q_OBJECT
public:
    static LoggerManager& getInstance();
    void registerManager(QTabWidget* loggerTabWidget);

    void addWidget(LoggerWidget* widget, QString tabName = "日志");
    void removeWidget(LoggerWidget* widget);

    void addLog(const QString &message);
    void addLog(const QString &message, int idx);
    void clearLog();

signals:
    void logAdded(const QString &message); // discard
    void logAddedToWidget(const QString &message, QWidget* targetWidget);
    void clearAllLogs();

private:
    QTabWidget *m_loggerTabWidget; // 被管理的日志窗口

    LoggerManager() = default;
    LoggerManager(const LoggerManager&) = delete;
    LoggerManager& operator=(const LoggerManager&) = delete;
};
#endif // LOGGERMANAGER_H
