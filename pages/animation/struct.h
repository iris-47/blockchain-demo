#ifndef ANIMATION_STRUCT_H
#define ANIMATION_STRUCT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>

#include <QDebug>

class Shard;

enum MessageType{
    // DEFAULT
    DEFAULT,

    // PBFT
    PROPOSE,
    PRE_PREPARE,
    PREPARE,
    COMMIT,

    // CShard
    VERIFY_REQUEST,
    VERIFIED,
    BLOCK,
};

class ColorMap{
public:
    static QColor getColor(MessageType type){
        static const QMap<MessageType, QColor> colorMap = {
                                                           {MessageType::DEFAULT, Qt::black},

                                                           {MessageType::PROPOSE, Qt::red},
                                                           {MessageType::PRE_PREPARE, Qt::magenta},
                                                           {MessageType::PREPARE, Qt::yellow},
                                                           {MessageType::COMMIT, Qt::green},

                                                           {MessageType::VERIFY_REQUEST, QColor(0xFF00FF)}, // 紫色
                                                           {MessageType::VERIFIED, Qt::green},
                                                           {MessageType::BLOCK, Qt::blue},
                                                           };


        auto it = colorMap.find(type);
        if(it != colorMap.end()){
            return it.value();
        }

        // default
        return Qt::white;
    }

    // 非常丑陋的妥协产物，请勿模仿
    static QMap<MessageType, QColor> getColorMap(){
        static const QMap<MessageType, QColor> colorMap = {
                                                           {MessageType::DEFAULT, Qt::black},

                                                           {MessageType::PROPOSE, Qt::red},
                                                           {MessageType::PRE_PREPARE, Qt::magenta},
                                                           {MessageType::PREPARE, Qt::yellow},
                                                           {MessageType::COMMIT, Qt::green},

                                                           {MessageType::VERIFY_REQUEST, QColor(0xFF00FF)}, // 紫色
                                                           {MessageType::VERIFIED, Qt::green},
                                                           {MessageType::BLOCK, Qt::blue},
                                                           };

        return colorMap;
    }
};

class Node : public QGraphicsEllipseItem
{
    friend class Shard;
public:
    Node(qreal x, qreal y, qreal r, QColor color = Qt::black, QGraphicsItem* parent = nullptr) : QGraphicsEllipseItem(parent),
        m_color(color), m_prepareCnt(0), m_commitCnt(0)
    {
        setRect(x, y, 2 * r, 2 * r);
        // 设置QGraphicsEllipseItem填充颜色
        // setBrush(QBrush(color));

        // 设置QGraphicsEllipseItem边缘颜色
        QPen pen;
        pen.setColor(color);
        pen.setWidth(3);
        setPen(pen);

        // setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

    void setShard(Shard* shard) {m_belongShard = shard;}
    Shard* getShard() {return m_belongShard;}

    void setColor(QColor color) { m_color = color; }
    void reset() { m_prepareCnt = m_commitCnt = 0; setBrush(Qt::white);}

private:
    QColor m_color;
    Shard* m_belongShard;

    int m_prepareCnt;
    int m_commitCnt;
};

class Message : public QGraphicsEllipseItem {
public:
    Message(qreal size=3, MessageType mtype = MessageType::DEFAULT, QGraphicsItem* parent = nullptr):
        QGraphicsEllipseItem(parent), mtype(mtype)
    {
        setRect(0, 0, size * 2, size * 2);
        setBrush(ColorMap::getColor(mtype));
    }

    /// @param from 起始中心点
    /// @param to 目标中心点
    Message(Node* from, Node* to, qreal size=3 , MessageType mtype = MessageType::DEFAULT, QGraphicsItem* parent = nullptr) :
    // Message(Node* from, Node* to, qreal size=3 , MessageType mtype = MessageType::DEFAULT, QGraphicsItem* parent = nullptr) :
        QGraphicsEllipseItem(parent)
        , target(to),  mtype(mtype)
        , targetp(to->pos() + to->boundingRect().center())
    {
        setRect(0, 0, size * 2, size * 2);
        setBrush(ColorMap::getColor(mtype));
    }

    Node* target; // 目标节点
    QPointF targetp; // 目标中心点
    MessageType mtype;
};
#endif // ANIMATION_STRUCT_H
