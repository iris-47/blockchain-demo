#ifndef ANIMATIONSCENE_H
#define ANIMATIONSCENE_H

#include <QGraphicsScene>
#include <QVector>
#include <QTimer>
#include "struct.h"
#include "shard.h"
#include "config.h"

class Node;
class Message;
class Shard;

class AnimationScene : public QGraphicsScene
{
    friend class AnimationWidget;

    Q_OBJECT;
public:
    explicit AnimationScene(QObject *parent = nullptr);

    void initScene();

    void addNode(qreal x, qreal y, qreal r, QColor color = Qt::black);
    void addShard(qreal x, qreal y, int nnm, QColor color = Qt::black);

    void sendMessage(Node *from, Node *to, MessageType mtype = MessageType::DEFAULT);
    void sendMessage(Node *from, Shard* to, MessageType mtype = MessageType::DEFAULT);
    void sendMessage(Shard* from, Node* to, MessageType mtype = MessageType::DEFAULT);
    void shardReply(Shard* from);

    void updateNodes();
    void updateScene();

    void setSpeed(qreal speed) { m_speed = speed; }

public slots:
    void updateMessages();

private:
    QVector<Node*> nodes;
    QVector<Message*> messages;
    QVector<Shard*> shards;
    QTimer *timer;

    qreal m_speed;
};

#endif // ANIMATIONSCENE_H
