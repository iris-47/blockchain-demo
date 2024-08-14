#include "loggerwidget.h"
#include <QScrollBar>
#include <QDateTime>
#include <QLabel>

void LoggerWidget::addText(const QString &text)
{
    QLabel *label = new QLabel(text, this);

    // 添加时间戳
    QDateTime current_date_time = QDateTime::currentDateTime();
    // 精确到毫秒
    QString current_date = current_date_time.toString("hh:mm:ss.zzz");
    label->setText(current_date + ": " + text);

    label->setStyleSheet("QLabel { background-color : lightgray; color : black; padding: 5px; border-radius: 5px; }");

    m_layout->insertWidget(0, label); // Insert new text at the top

    // Auto-scroll to the top
    m_scrollArea->verticalScrollBar()->setValue(0);
}

LoggerWidget::LoggerWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_scrollArea = new QScrollArea(this);
    m_scrollWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_scrollWidget);

    m_layout->setAlignment(Qt::AlignTop);
    m_scrollWidget->setLayout(m_layout);
    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);
}

