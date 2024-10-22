#include "loggermanager.h"
#include <QDebug>

void LoggerManager::registerManager(QTabWidget *loggerTabWidget)
{
    m_loggerTabWidget = loggerTabWidget;
    loggerTabWidget->removeTab(0);
    loggerTabWidget->removeTab(0); // 删除自带的两个Tab
}

void LoggerManager::addLog(const QString &message)
{
    if (m_loggerTabWidget->count() == 0)
    {
        qDebug() << "LoggerManager: no logger widget found";
        return;
    }
    emit logAddedToWidget(message, m_loggerTabWidget->widget(0)); // the main logger
}

void LoggerManager::addLog(const QString &message, int idx)
{
    if(m_loggerTabWidget->count() > idx)
    {
        emit logAddedToWidget(message, m_loggerTabWidget->widget(idx));
    }
    else
    {
        qDebug() << "LoggerManager: shardIndex out of range";
    }
}

void LoggerManager::clearLog()
{
    emit clearAllLogs();
}

void LoggerManager::addWidget(LoggerWidget *widget, QString tabName)
{
    if(!widget){
        return;
    }

    connect(this, &LoggerManager::logAddedToWidget, widget, [=](const QString &message, QWidget* targetWidget){
        if(widget == targetWidget){
            widget->addText(message); // 仅发送到目标widget
        }
    });
    connect(this, &LoggerManager::clearAllLogs, widget, &LoggerWidget::clearText); // 清空所有Tab的日志

    m_loggerTabWidget->addTab(widget, tabName);
    qDebug() << "LoggerManager: connected widget";
}

// BUG::断开不同的 Widget 导致 index 错位
void LoggerManager::removeWidget(LoggerWidget *widget)
{
    if (!widget) return;

    // 从m_loggerTabWidget中删除widget
    for(int i = 0; i < m_loggerTabWidget->count(); i++)
    {
        // 断开连接
        disconnect(this, &LoggerManager::logAddedToWidget, widget, 0);
        disconnect(this, &LoggerManager::clearAllLogs, widget, 0);

        // 移除Tab
        if(m_loggerTabWidget->widget(i) == widget)
        {
            m_loggerTabWidget->removeTab(i);
            break;
        }

        free(widget);
    }
}

LoggerManager& LoggerManager::getInstance()
{
    static LoggerManager instance;
    return instance;
}
