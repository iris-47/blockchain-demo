#include "animationscene.h"
#include "struct.h"
#include "shard.h"

#include <QVector2D>
#include <QDebug>

AnimationScene::AnimationScene(QObject *parent)
    : QGraphicsScene{parent}, timer{new QTimer{this}}
{
    connect(timer, &QTimer::timeout, this, &AnimationScene::updateScene);
    m_speed = 1;
    timer->start(1000/30);
}

void AnimationScene::addNode(qreal x, qreal y, qreal r, QColor color)
{
    Node *node = new Node{0, 0, r, color};
    addItem(node);
    node->setPos(x, y);
    nodes.append(node);
}

void AnimationScene::sendMessage(Node* from, Node* to){
    // pos()获取的是Scnene坐标系坐标，boundingRect()获取的是Item的坐标系坐标，坑
    QPointF from_p = from->pos() + from->boundingRect().center();

    Message *message = new Message(from, to);

    addItem(message);
    message->setPos(from_p);
    messages.append(message);
}

void AnimationScene::updateMessages() {
    for (int i = 0; i < messages.size(); ++i) {
        QPointF end = messages[i]->targetp;
        QPointF currentPos = messages[i]->pos();
        QVector2D direction(end - currentPos);
        // 如果消息已经到达终点
        if (QVector2D(currentPos - end).length() < m_speed) {
            removeItem(messages[i]);
            delete messages[i];
            messages.removeAt(i);
            --i;
        }else{
            direction.normalize();
            messages[i]->setPos(currentPos + direction.toPointF() * m_speed);
        }
    }
}

void AnimationScene::addShard(qreal x, qreal y, int nnm, QColor color){
    Shard *shard = new Shard{0, 0, nnm, color};
    shards.append(shard);

    addItem(shard->group); // 作为整体添加
    shard->group->setPos(x, y);
}

void AnimationScene::updateScene(){
    for(int i = 0;i < shards.size();i++){
        shards[i]->updateMessages();
    }

    updateMessages();

}

// // For Test
// void AnimationScene::updateNodes(){
//     for(int i = 0;i < nodes.size();i++){
//         Node *node = nodes.at(i);
//         node->setPos(node->pos() + QPointF(1, 1));
//     }
// }
