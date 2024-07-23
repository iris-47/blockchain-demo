#include "shard.h"
#include "config.h"
#include <random>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <QVector2D>

// 检测节点是否重叠
bool isNonOverlapping(qreal x, qreal y, QVector<Node*> nodes, float minDistance) {
    for(int i = 0;i < nodes.size();i++) {
        qreal distance = std::sqrt(std::pow(nodes[i]->x() - x + CONFIG::NODE_RADIUS, 2) + std::pow(nodes[i]->y() - y + CONFIG::NODE_RADIUS, 2));
        if (distance < minDistance) {
            return false;
        }
    }

    return true;
}

Shard::Shard() {}
Shard::Shard(qreal x, qreal y, int nnm, QColor color) :
    m_radius((std::sqrt(nnm) + 1) * CONFIG::RADIUS_RATE),
    m_speed(CONFIG::INNER_MESSAGE_SPEED)
{
    connect(&replyAnimationTimer, &QTimer::timeout, this, &Shard::reply);

    setRect(x, y, m_radius * 2, m_radius * 2);

    QPen pen = QPen(color);
    pen.setWidth(3);
    setPen(pen);

    group = new QGraphicsItemGroup();
    group->addToGroup(this);
    group->setFlag(QGraphicsItem::ItemIsSelectable);
    group->setFlag(QGraphicsItem::ItemIsMovable);

    qreal nodeRadius = CONFIG::NODE_RADIUS; // 节点半径
    qreal innerRadius = m_radius - nodeRadius - CONFIG::SHARD_BORDER; // 减去节点半径和边缘间距

    // 随机布局节点
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<qreal> dis(0, 1);

    qreal r = innerRadius * std::sqrt(dis(gen));
    qreal angle = 2 * M_PI * dis(gen);
    qreal node_center_x = rect().center().x() + r * std::cos(angle);
    qreal node_center_y = rect().center().y() + r * std::sin(angle);
    Node* node = new Node(0, 0, nodeRadius, Qt::darkCyan);

    group->addToGroup(node);
    node->setPos(node_center_x - nodeRadius, node_center_y - nodeRadius);
    nodes.append(node);
    m_mainNode = node;

    // 节点类的坐标是左上角，所以转换得有点扭曲。。。
    while(nodes.size() < nnm) {
        qreal r = innerRadius * std::sqrt(dis(gen));
        qreal angle = 2 * M_PI * dis(gen);
        qreal node_center_x = rect().center().x() + r * std::cos(angle);
        qreal node_center_y = rect().center().y() + r * std::sin(angle);
        if(isNonOverlapping(node_center_x, node_center_y, nodes, CONFIG::NODE_RADIUS * 4)){
            Node* node = new Node(0, 0, nodeRadius);
            group->addToGroup(node);
            node->setPos(node_center_x - nodeRadius, node_center_y - nodeRadius);
            nodes.append(node);
        }
    }
}

// Group视作一个小的scene
void Shard::sendMessage(Node* from, Node* to, MessageType mtype) {
    Message *message = new Message(from, to, CONFIG::MESSAGE_SIZE, mtype);

    group->addToGroup(message);
    message->setPos(from->pos() + from->boundingRect().center());
    messages.append(message);
}

void Shard::sendMessage(int from, int to, MessageType mtype) {
    if(from > nodes.size() || to > nodes.size()){
        qDebug() << "from or to is out of range!";
        return;
    }
    sendMessage(nodes[from], nodes[to], mtype);
}

void Shard::broadcastMessage(Node* from, MessageType mtype) {
    QPointF from_p = from->pos() + from->boundingRect().center();
    for(int i = 0; i < nodes.size(); i++){
        if(from != nodes[i]){
            Message *message = new Message(from, nodes[i], CONFIG::MESSAGE_SIZE, mtype);

            group->addToGroup(message);
            message->setPos(from_p);
            messages.append(message);
        }
    }
}

void Shard::broadcastMessage(int from, MessageType mtype) {
    if(from > nodes.size()){
        qDebug() << "from is out of range!";
        return;
    }
    broadcastMessage(nodes[from], mtype);
}

void Shard::updateMessages() {
    for (int i = 0; i < messages.size(); ++i) {
        QPointF end = messages[i]->targetp;
        QPointF currentPos = messages[i]->pos();
        QVector2D direction(end - currentPos);
        // 如果消息已经到达终点
        if (QVector2D(currentPos - end).length() < m_speed) {
            handleMessage(messages[i]->target, messages[i]);

            // handleMessage中可能删除所有消息
            if(messages.size() == 0){
                return;
            }

            group->removeFromGroup(messages[i]);
            delete messages[i];
            messages.removeAt(i);
            --i;
        }else{
            direction.normalize();
            // qDebug() << "currentPos: " << currentPos << " end: " << end << " direction: " << direction.toPointF();
            messages[i]->setPos(currentPos + direction.toPointF() * m_speed);
        }
    }
}

void Shard::handleMessage(Node* node, Message* message) {
    if(node == nullptr || message == nullptr){
        return;
    }

    switch (message->mtype) {
        case MessageType::PRE_PREPARE:
            node->setBrush(ColorMap::getColor(message->mtype));
            broadcastMessage(node, MessageType::PREPARE);
            break;
        case MessageType::PREPARE:
            node->m_prepareCnt++;
            if(node->m_prepareCnt >= 2 * nodes.size() / 3){
                node->m_prepareCnt = 0;
                node->setBrush(ColorMap::getColor(message->mtype));
                broadcastMessage(node, MessageType::COMMIT);
            }
            break;
        case MessageType::COMMIT:
            node->m_commitCnt++;
            if(node->m_commitCnt >= 2 * nodes.size() / 3){
                node->m_commitCnt = 0;
                node->setBrush(ColorMap::getColor(message->mtype));
                if(node == m_mainNode){
                    consensusDone();
                }
            }
            break;
        default:
            break;
    }
}

void Shard::startPBFT() {
    broadcastMessage(0, MessageType::PRE_PREPARE);
}

void Shard::consensusDone(){
    setBrush(Qt::green);
    if(replyAnimationTimer.isActive()){
        replyAnimationTimer.stop();
    }
    replyAnimationTimer.start(CONFIG::REPLY_ANIMATION_INTERVAL);

    //删除所有消息
    for(int i = 0; i < messages.size(); i++){
        qDebug() << "remove message " << i;
        group->removeFromGroup(messages[i]);
        delete messages[i];
    }

    messages.clear();

    for(int i = 0;i < nodes.size();i++){
        nodes[i]->reset();
        nodes[i]->setVisible(false);
    }
}

void Shard::reply(){
    if(replyAnimationTimer.isActive()){
        replyAnimationTimer.stop();
    }
    for(int i = 0;i < nodes.size();i++){
        nodes[i]->setVisible(true);
    }
    setBrush(Qt::transparent);
}
