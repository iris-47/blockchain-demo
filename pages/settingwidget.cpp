#include "settingwidget.h"
#include "ui_settingwidget.h"

#include "config.h"

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingWidget)
{
    ui->setupUi(this);

    ui->frameRateCombo->setCurrentIndex(1);

    ui->intraMsgSpeedSlider->setMinimum(CONFIG::INNER_MESSAGE_SPEED * 0.2);
    ui->intraMsgSpeedSlider->setMaximum(CONFIG::INNER_MESSAGE_SPEED * 5);
    ui->intraMsgSpeedSlider->setValue(CONFIG::INNER_MESSAGE_SPEED);
    connect(ui->intraMsgSpeedSlider, &QSlider::valueChanged, this,  [=](int value){
        ui->intraMsgSpeedLabel->setText(QString::number(value));
    });

    ui->interMsgSpeedSlider->setMinimum(CONFIG::MESSAGE_SPEED * 0.2);
    ui->interMsgSpeedSlider->setMaximum(CONFIG::MESSAGE_SPEED * 5);
    ui->interMsgSpeedSlider->setValue(CONFIG::MESSAGE_SPEED);
    connect(ui->interMsgSpeedSlider, &QSlider::valueChanged, this, [=](int value){
        ui->interMsgSpeedLabel->setText(QString::number(value));
    });

    ui->startIndexSpin->setMinimum(0);
    ui->startIndexSpin->setMaximum(CONFIG::SHARD_NUM - 1);
    ui->startIndexSpin->setValue(0);

    ui->endIndexBox->clear();
    for(int i = 0; i < CONFIG::SHARD_NUM; i++)
        ui->endIndexBox->addItem(QString::number(i));
    ui->endIndexBox->SetPlaceHolderText("选择分片Index");
    ui->endIndexBox->setCurrentText("1; 3");

    connect(ui->applyButton, &QPushButton::clicked, this, &SettingWidget::applyChanges);
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::applyChanges()
{
    CONFIG::FRAME_RATE = ui->frameRateCombo->currentText().chopped(4).toInt();
    CONFIG::INNER_MESSAGE_SPEED = ui->intraMsgSpeedSlider->value();
    CONFIG::MESSAGE_SPEED = ui->interMsgSpeedSlider->value();
    CONFIG::START_INDEX = ui->startIndexSpin->value();
    CONFIG::END_INDEX.clear();
    QStringList endList = ui->endIndexBox->currentText().split(";");
    for(int i = 0; i < endList.size(); i++){
        CONFIG::END_INDEX.push_back(endList[i].toInt());
    }

    emit settingChanged();
}
