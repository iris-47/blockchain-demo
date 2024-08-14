#include "animationwidget.h"
#include "ui_animationwidget.h"

#include "animation/animationscene.h"
#include "animation/legenditem.h"

AnimationWidget::AnimationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnimationWidget)
    , scene(new AnimationScene(this))
    , m_isRunning(false)
{
    ui->setupUi(this);

    LegendItem* legend = new LegendItem();

    scene->initScene();
    scene->addItem(legend);
    legend->setInitialPos(QPointF(-800, 0));

    ui->startBtn->setIcon(QIcon(":/icons/start.png"));
    ui->resetBtn->setIcon(QIcon(":/icons/reset.png"));

    initConnections();

    // scene->startDemo();

    // ui->graphicsView = new GraphicsView(scene);
    ui->graphicsView->setScene(scene);
}

void AnimationWidget::initConnections()
{
    connect(ui->startBtn, &QPushButton::clicked, this, &AnimationWidget::onClickedStart);
    connect(ui->resetBtn, &QPushButton::clicked, this, &AnimationWidget::onClickReset);
}

void AnimationWidget::onClickedStart()
{
    m_isRunning = !m_isRunning;
    if(m_isRunning){
        ui->startBtn->setIcon(QIcon(":/icons/pause.png"));
    }else{
        ui->startBtn->setIcon(QIcon(":/icons/start.png"));
    }
    scene->startDemo();
}

void AnimationWidget::onClickReset(){
    m_isRunning = false;
    ui->startBtn->setIcon(QIcon(":/icons/start.png"));
    scene->resetScene();
}

AnimationWidget::~AnimationWidget()
{
    delete ui;
}
