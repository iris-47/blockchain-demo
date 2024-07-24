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

    scene->sendMessage(scene->shards[0]->nodes[0], scene->shards[1], MessageType::PROPOSE);
    scene->sendMessage(scene->shards[0]->nodes[0], scene->shards[3], MessageType::PROPOSE);
    // ui->graphicsView = new GraphicsView(scene);
    ui->graphicsView->setScene(scene);

}

AnimationWidget::~AnimationWidget()
{
    delete ui;
}
