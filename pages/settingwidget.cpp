#include "settingwidget.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QListWidget>
#include <QToolBox>
#include <QCheckBox>
#include <QPushButton>


SettingWidget::SettingWidget(QWidget *parent)
    : QWidget(parent)
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 帧率设置
    QGroupBox *frameRateGroup = new QGroupBox("帧率设置");
    QVBoxLayout *frameRateLayout = new QVBoxLayout;
    QComboBox *frameRateCombo = new QComboBox();
    frameRateCombo->addItem("20 FPS");
    frameRateCombo->addItem("30 FPS");
    frameRateCombo->addItem("60 FPS");
    frameRateCombo->setCurrentIndex(1); // 默认设置为30 FPS
    frameRateLayout->addWidget(frameRateCombo);
    frameRateGroup->setLayout(frameRateLayout);

    // 片内消息速度设置
    QGroupBox *intraMsgSpeedGroup = new QGroupBox("片内消息速度设置");
    QVBoxLayout *intraMsgSpeedLayout = new QVBoxLayout;
    QSlider *intraMsgSpeedSlider = new QSlider(Qt::Horizontal);
    intraMsgSpeedSlider->setRange(0, 1000);  // 设置滑动条范围
    intraMsgSpeedSlider->setValue(100);      // 默认值
    QLabel *intraMsgSpeedLabel = new QLabel("100");
    connect(intraMsgSpeedSlider, &QSlider::valueChanged, [intraMsgSpeedLabel](int value) {
        intraMsgSpeedLabel->setText(QString::number(value));
    });
    intraMsgSpeedLayout->addWidget(intraMsgSpeedSlider);
    intraMsgSpeedLayout->addWidget(intraMsgSpeedLabel);
    intraMsgSpeedGroup->setLayout(intraMsgSpeedLayout);

    // 片间消息速度设置
    QGroupBox *interMsgSpeedGroup = new QGroupBox("片间消息速度设置");
    QVBoxLayout *interMsgSpeedLayout = new QVBoxLayout;
    QSlider *interMsgSpeedSlider = new QSlider(Qt::Horizontal);
    interMsgSpeedSlider->setRange(0, 1000);  // 设置滑动条范围
    interMsgSpeedSlider->setValue(200);      // 默认值
    QLabel *interMsgSpeedLabel = new QLabel("200");
    connect(interMsgSpeedSlider, &QSlider::valueChanged, [interMsgSpeedLabel](int value) {
        interMsgSpeedLabel->setText(QString::number(value));
    });
    interMsgSpeedLayout->addWidget(interMsgSpeedSlider);
    interMsgSpeedLayout->addWidget(interMsgSpeedLabel);
    interMsgSpeedGroup->setLayout(interMsgSpeedLayout);

    // 初始化表设置
    QGroupBox *initTableGroup = new QGroupBox("初始化表设置");
    QVBoxLayout *initTableLayout = new QVBoxLayout;
    QFormLayout *initIndexLayout = new QFormLayout;

    // 添加提示信息
    QLabel *startIndexTip = new QLabel("请设置起始 Index，用于指定初始化的起始位置：");
    QLabel *targetIndexTip = new QLabel("请设置目标 Indexes，您可以选择多个目标 Index 进行操作：");

    // 起始 index 设置
    QSpinBox *startIndexSpin = new QSpinBox();
    startIndexSpin->setMinimum(0);
    startIndexSpin->setMaximum(100);
    startIndexSpin->setValue(10);  // 默认起始 index
    initIndexLayout->addRow("起始 Index：", startIndexSpin);

    // 目标 indexs 设置（折叠式）
    QToolBox *targetIndexToolbox = new QToolBox();
    QWidget *targetIndexWidget = new QWidget();
    QVBoxLayout *targetIndexLayout = new QVBoxLayout(targetIndexWidget);

    for (int i = 0; i < 10; ++i) {
        QCheckBox *indexCheckBox = new QCheckBox(QString("目标 %1").arg(i));
        targetIndexLayout->addWidget(indexCheckBox);
    }

    targetIndexToolbox->addItem(targetIndexWidget, "选择目标 Indexs");

    // 将提示信息和设置控件添加到布局中
    initTableLayout->addWidget(startIndexTip);
    initTableLayout->addLayout(initIndexLayout);
    initTableLayout->addWidget(targetIndexTip);
    initTableLayout->addWidget(targetIndexToolbox);

    initTableGroup->setLayout(initTableLayout);

    // 添加到主布局
    mainLayout->addWidget(frameRateGroup);
    mainLayout->addWidget(intraMsgSpeedGroup);
    mainLayout->addWidget(interMsgSpeedGroup);
    mainLayout->addWidget(initTableGroup);

    // 确定按钮
    QPushButton *applyButton = new QPushButton("应用设置");
    mainLayout->addWidget(applyButton);

    setLayout(mainLayout);
    setWindowTitle("设置窗口");
}
