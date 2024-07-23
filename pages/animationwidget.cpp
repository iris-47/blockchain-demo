#include "animationwidget.h"
#include "ui_animationwidget.h"

#include "animation/animationscene.h"
#include "animation/shard.h"
AnimationWidget::AnimationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnimationWidget)
    , scene(new AnimationScene(this))
{
    ui->setupUi(this);

    // 绘制带刻度的坐标系
    scene->addLine(-1000, 0, 1000, 0, QPen(Qt::gray));
    scene->addLine(0, -1000, 0, 1000, QPen(Qt::gray));
    for (int i = -1000; i <= 1000; i += 20) {
        scene->addLine(i, -5, i, 5, QPen(Qt::gray));
        scene->addLine(-5, i, 5, i, QPen(Qt::gray));
    }

    scene->addNode(0, 0, 20, Qt::red);
    scene->addNode(100, 100, 20, Qt::green);
    scene->addNode(200, 200, 40);

    scene->sendMessage(scene->nodes[0], scene->nodes[1]);
    scene->sendMessage(scene->nodes[2], scene->nodes[1]);

    scene->addShard(300, 300, 10);
    scene->addShard(400, 800, 6);

    scene->shards[0]->startPBFT();

    // ui->graphicsView = new GraphicsView(scene);
    ui->graphicsView->setScene(scene);

}

AnimationWidget::~AnimationWidget()
{
    delete ui;
}
