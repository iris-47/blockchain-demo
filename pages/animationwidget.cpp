#include "animationwidget.h"
#include "ui_animationwidget.h"

#include "animation/animationscene.h"
#include "animation/struct.h"
// #include "animation/shard.h"
AnimationWidget::AnimationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnimationWidget)
    , scene(new AnimationScene(this))
{
    ui->setupUi(this);


    scene->initScene();
    connect(ui->startBtn, &QPushButton::clicked, scene, &AnimationScene::startDemo);
    connect(ui->resetBtn, &QPushButton::clicked, scene, &AnimationScene::resetScene);

    scene->startDemo();

    // ui->graphicsView = new GraphicsView(scene);
    ui->graphicsView->setScene(scene);
}

AnimationWidget::~AnimationWidget()
{
    delete ui;
}
