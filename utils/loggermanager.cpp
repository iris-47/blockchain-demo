#include "loggermanager.h"
#include <QDebug>

void LoggerManager::addLog(const QString &message)
{
    if(!m_isConnected){
        qDebug() << "LoggerManager: not connected";
        return;
    }
    emit logAdded(message);
}

void LoggerManager::connectWidget(LoggerWidget *widget)
{
    if(m_isConnected)
    {
        qDebug() << "LoggerManager: already connected";
        return;
    }
    connect(this, &LoggerManager::logAdded, widget, &LoggerWidget::addText);
    m_isConnected = true;
}

// BUG::断开不同的Widget导致重复添加连接
void LoggerManager::disconnetWidget(LoggerWidget *widget)
{
    if (m_isConnected)
    {
        disconnect(this, &LoggerManager::logAdded, widget, &LoggerWidget::addText);
        m_isConnected = false;
    }
}

LoggerManager& LoggerManager::getInstance()
{
    static LoggerManager instance;
    return instance;
}
