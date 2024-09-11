#ifndef SHARD_H
#define SHARD_H

#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QMutex>
#include "struct.h"

class AnimationScene;

// TODO: 尝试换一个东西继承，而不是继承QGraphicsEllipseItem
// URGENT: 点击显示详细信息
class Shard : public QObject, public QGraphicsEllipseItem
{
    friend class AnimationScene;
    friend class AnimationWidget;
    Q_OBJECT
public:
    Shard();
    Shard(qreal x, qreal y, int nnm, int index, QColor color = Qt::black);

    void sendMessage(Node* from, Node* to, MessageType mtype = MessageType::DEFAULT);
    void sendMessage(int from, int to, MessageType mtype = MessageType::DEFAULT);
    void broadcastMessage(Node* from, MessageType mtype = MessageType::DEFAULT);
    void broadcastMessage(int from, MessageType mtype = MessageType::DEFAULT);

    void updateMessages();
    void resetShard();

    void handleMessage(Node* node, Message* message);
    void startPBFT(Node* node);
    void consensusDone();

    void setSpeed(qreal speed) { m_speed = speed; }


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    QRectF boundingRect() const;

public slots:
    void reply();
    void resetSlot();

private:
    qreal m_radius;

    AnimationScene* m_belongScene;

    QGraphicsItemGroup *group; // 所有Item应该加入到group中，方便拖动
    QVector <Node*> nodes;
    QVector <Message*> messages;
    QMutex messages_mutex; // 用于reset时的 messages 保护

    Node* m_mainNode; // 主节点

    QTimer consensusTimer;
    QTimer replyAnimationTimer;
    qreal m_speed;

    QColor m_penColor;
    QColor m_brushColor;

    int idx;
};

#endif // SHARD_H
