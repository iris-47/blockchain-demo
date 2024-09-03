#include "usagewidget.h"
#include "ui_usagewidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QGroupBox>
#include <QWidget>
#include <QLabel>

UsageWidget::UsageWidget(QStackedWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UsageWidget)
{
    ui->setupUi(this);

    QVBoxLayout *contentLayout = new QVBoxLayout;

    ui->contentWidget->setStyleSheet("background-color: #f0f0f0; padding: 10px;");

    // 快捷键
    QGroupBox *shortcutGroup = new QGroupBox("快捷键", ui->contentWidget);
    QVBoxLayout *shortcutLayout = new QVBoxLayout(shortcutGroup);
    shortcutLayout->addWidget(new QLabel("鼠标滚轮: 缩放画面", shortcutGroup));
    shortcutLayout->addWidget(new QLabel("鼠标左键: 拖动画布", shortcutGroup));
    shortcutLayout->addWidget(new QLabel("Ctrl + 鼠标左键: 拖动节点", shortcutGroup));

    // 功能
    QGroupBox *featureGroup = new QGroupBox("功能", ui->contentWidget);
    QVBoxLayout *featureLayout = new QVBoxLayout(featureGroup);
    featureLayout->addWidget(new QLabel("// 解释产品功能的语句", featureGroup));

    contentLayout->addWidget(shortcutGroup);
    contentLayout->addWidget(featureGroup);
    contentLayout->addStretch(1);
    ui->contentWidget->setLayout(contentLayout);

    // 样式设置
    QString groupBoxStyle = "QGroupBox { font-weight: bold; color: #004080; border: 1px solid #cccccc; border-radius: 5px; margin-top: 1ex; }"
                            "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px; }";
    shortcutGroup->setStyleSheet(groupBoxStyle);
    featureGroup->setStyleSheet(groupBoxStyle);
}

UsageWidget::~UsageWidget()
{
    delete ui;
}
