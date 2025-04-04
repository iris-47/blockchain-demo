#include "legendwidget.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

LegendItem::LegendItem(const QColor &color, const QString &text, QWidget *parent)
    : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    // 创建颜色块
    QFrame *colorBox = new QFrame();
    colorBox->setFixedSize(20, 20);
    colorBox->setStyleSheet(QString("background-color: %1; border-radius: 3px; border: 1px solid #333;")
                                .arg(color.name()));

    // 文字标签
    QLabel *label = new QLabel(text);
    label->setStyleSheet("font-size: 14px; color: #333;");

    // 将颜色块和标签放入布局
    layout->addWidget(colorBox);
    layout->addWidget(label);
    layout->addStretch();
}

LegendWidget::LegendWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedWidth(200);
    // setStyleSheet("background-color: #f9f9f9; border-radius: 10px; ");
    // 左侧内边距

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(25);

    // 添加多个图例项
    mainLayout->addWidget(new LegendItem(Qt::black, "Default"));
    mainLayout->addWidget(new LegendItem(Qt::red, "PROPOSE 消息"));
    mainLayout->addWidget(new LegendItem(Qt::magenta, "PRE-PREPARE 消息"));
    mainLayout->addWidget(new LegendItem(Qt::yellow, "PREPARE 消息"));
    mainLayout->addWidget(new LegendItem(Qt::green, "COMMIT 消息"));
    mainLayout->addWidget(new LegendItem(Qt::darkGreen, "VERIFIED 消息"));

    mainLayout->addStretch();

    setLayout(mainLayout);
}
