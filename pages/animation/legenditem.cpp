#include "legenditem.h"
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QPainter>
#include <QPushButton>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

LegendItem::LegendItem(QGraphicsItem *parent) : QGraphicsWidget(parent), m_isCollapsed(false)
{
    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainLayout);

    QGraphicsLinearLayout* buttonLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    // buttonLayout布局为两个按钮在最右侧
    buttonLayout->addStretch(1);

    QPushButton* collapseButton = new QPushButton();
    collapseButton->setIcon(QIcon(":/icons/collapse.png"));
    connect(collapseButton, &QPushButton::clicked, this, &LegendItem::toggleCollapse);
    QGraphicsProxyWidget* collapseProxy = new QGraphicsProxyWidget();
    collapseProxy->setWidget(collapseButton);
    buttonLayout->addItem(collapseProxy);

    QPushButton* closeButton = new QPushButton();
    closeButton->setIcon(QIcon(":/icons/close.png"));
    connect(closeButton, &QPushButton::clicked, this, &LegendItem::closeLegend);
    QGraphicsProxyWidget* closeProxy = new QGraphicsProxyWidget();
    closeProxy->setWidget(closeButton);
    buttonLayout->addItem(closeProxy);

    mainLayout->addItem(buttonLayout);

    // 遍历colormap
    auto colorMap = ColorMap::getColorMap();

    for(auto type : colorMap.keys()){
        QGraphicsLinearLayout* itemLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        QLabel* colorLabel = new QLabel();
        colorLabel->setFixedSize(20, 20);
        colorLabel->setStyleSheet(QString("background-color : %1;").arg(ColorMap::getColor(type).name()));
        QGraphicsProxyWidget* colorProxy = new QGraphicsProxyWidget();
        colorProxy->setWidget(colorLabel);
        itemLayout->addItem(colorProxy);

        QLabel* textLabel  = new QLabel(typeToString(type));
        QGraphicsProxyWidget* textProxy = new QGraphicsProxyWidget();
        textProxy->setWidget(textLabel);
        itemLayout->addItem(textProxy);

        mainLayout->addItem(itemLayout);
    }
}


void LegendItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::lightGray);
    painter->drawRect(boundingRect());
}

void LegendItem::toggleCollapse(){
    m_isCollapsed = !m_isCollapsed;
    for (int i = 1; i < layout()->count(); ++i) {
        layout()->itemAt(i)->graphicsItem()->setVisible(!m_isCollapsed);
    }
}

void LegendItem::closeLegend(){
    emit legendClosed();
    hide();
}
