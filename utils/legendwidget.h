#ifndef LEGENDWIDGET_H
#define LEGENDWIDGET_H

#include <QWidget>

// 自定义图例项（LegendItem）
class LegendItem : public QWidget {
public:
    LegendItem(const QColor &color, const QString &text, QWidget *parent = nullptr);
};

class LegendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LegendWidget(QWidget *parent = nullptr);

signals:
};

#endif // LEGENDWIDGET_H
