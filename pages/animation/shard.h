#ifndef SHARD_H
#define SHARD_H

#include <QGraphicsEllipseItem>
#include <QTimer>
#include "struct.h"

class AnimationScene;

// TODO: 尝试换一个东西继承，而不是继承QGraphicsEllipseItem
class Shard : public QObject, public QGraphicsEllipseItem
{
    friend class AnimationScene;
    friend class AnimationWidget;
    Q_OBJECT
public:
    Shard();
    Shard(qreal x, qreal y, int nnm, QColor color = Qt::black);

    void sendMessage(Node* from, Node* to, MessageType mtype = MessageType::DEFAULT);
    void sendMessage(int from, int to, MessageType mtype = MessageType::DEFAULT);
    void broadcastMessage(Node* from, MessageType mtype = MessageType::DEFAULT);
    void broadcastMessage(int from, MessageType mtype = MessageType::DEFAULT);

    void updateMessages();

    void handleMessage(Node* node, Message* message);
    void startPBFT();
    void consensusDone();

public slots:
    void reply();

private:
    qreal m_radius;

    AnimationScene* m_belongScene;

    QGraphicsItemGroup *group; // 所有Item应该加入到group中，方便拖动
    QVector <Node*> nodes;
    QVector <Message*> messages;

    Node* m_mainNode;

    QTimer consensusTimer;
    QTimer replyAnimationTimer;
    qreal m_speed;
};

#endif // SHARD_H
