#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QWidget>
#include <QMutex>

class LoggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoggerWidget(QWidget *parent = nullptr);

public slots:
    void addText(const QString &message);

private:
    QVBoxLayout* m_layout;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollWidget;
};


#endif // LOGGERWIDGET_H
