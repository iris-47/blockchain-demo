#include "animationwidget.h"
#include "ui_animationwidget.h"

#include "utils/loggermanager.h"
#include "animation/animationscene.h"

#include <QPropertyAnimation>

AnimationWidget::AnimationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnimationWidget)
    , scene(new AnimationScene(this))
    , m_isRunning(false)
    , m_legendVisible(false)
{
    ui->setupUi(this);

    ui->startBtn->setIcon(QIcon(":/icons/start.png"));
    ui->clearBtn->setIcon(QIcon(":/icons/clear.png"));
    ui->legendBtn->setIcon(QIcon(":/icons/legend.png"));
    ui->resetBtn->setIcon(QIcon(":/icons/reset.png"));


    // ui->graphicsView = new GraphicsView(scene);
    ui->graphicsView->setScene(scene);
}

void AnimationWidget::init(){
    scene->initScene();
    initConnections();
}

void AnimationWidget::initConnections()
{
    connect(ui->startBtn, &QPushButton::clicked, this, &AnimationWidget::onClickedStart);
    connect(ui->resetBtn, &QPushButton::clicked, this, &AnimationWidget::onClickReset);
    connect(ui->legendBtn, &QPushButton::clicked, this, &AnimationWidget::onClickLegendBtn);
    connect(ui->clearBtn, &QPushButton::clicked, this, &AnimationWidget::onClickClear);
}

void AnimationWidget::onClickClear()
{
    // 清空日志
    LoggerManager::getInstance().clearLog();
}

void AnimationWidget::onClickedStart()
{
    m_isRunning = !m_isRunning;
    if(m_isRunning){
        ui->startBtn->setIcon(QIcon(":/icons/pause.png"));
        ui->startBtn->setText("暂停");
    }else{
        ui->startBtn->setIcon(QIcon(":/icons/start.png"));
        ui->startBtn->setText("开始");
    }
    scene->startDemo();
}

void AnimationWidget::onClickReset(){
    m_isRunning = false;
    ui->startBtn->setIcon(QIcon(":/icons/start.png"));
    scene->resetScene();
}

void AnimationWidget::onClickLegendBtn(){
    LegendWidget* legend = ui->legendWidget;
    // 隐藏图例，显示动画
    if (m_legendVisible) {
        // 隐藏图例并让 QGraphicsView 占据全部区域
        ui->grahicsLayout->removeWidget(legend); // 从布局中移除 LegendWidget
        legend->hide();
        ui->legendBtn->setText("显示图例");
    } else {
        // 重新显示图例，QGraphicsView 让出位置
        ui->grahicsLayout->addWidget(legend); // 将 LegendWidget 添加回布局
        legend->show();
        ui->legendBtn->setText("隐藏图例");
    }
    m_legendVisible = !m_legendVisible;
}

void AnimationWidget::onConfigChanged(int type){
    switch (type) {
        case FPS_CHANGED:
            scene->restartTimer();
            break;
        case SPEED_CHANGED:
            scene->setSpeed();
            break;
        case SETTING_CHANGED:
            onClickReset();
            scene->setConsensus();
        default:
            break;
    }
}

AnimationWidget::~AnimationWidget()
{
    delete ui;
}


