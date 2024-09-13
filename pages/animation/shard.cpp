#include "shard.h"
#include "config.h"
#include "animationscene.h"
#include "qstyleoption.h"
#include "utils/loggermanager.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRandomGenerator>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
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

void CustomItemGroup::mousePressEvent(QGraphicsSceneMouseEvent *event){
    // 将事件传递给第一个子Item
    // URGENT 创建详情对话框
    // int idx = 1;
    // QDialog dialog;
    // dialog.setWindowTitle(QString("Shard %1 Details").arg(idx));

    // QVBoxLayout *layout = new QVBoxLayout(&dialog);
    // QLabel *infoLabel = new QLabel(QString("Details for Shard %1").arg(idx));
    // layout->addWidget(infoLabel);

    // QPushButton *closeButton = new QPushButton("Close");
    // layout->addWidget(closeButton);

    // connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // // 显示详情对话框
    // dialog.exec();
    QGraphicsItemGroup::mousePressEvent(event);
}

Shard::Shard() {}
Shard::Shard(qreal x, qreal y, int nnm, int index, QColor color) :
    m_radius((std::sqrt(nnm) + 1) * CONFIG::RADIUS_RATE),
    m_speed(CONFIG::INNER_MESSAGE_SPEED),
    idx(index)
{
    connect(&replyAnimationTimer, &QTimer::timeout, this, &Shard::reply);

    setRect(x, y, m_radius * 2, m_radius * 2);

    m_penColor = color;
    m_brushColor = Qt::transparent;
    // QPen pen = QPen(color);
    // pen.setWidth(3);
    // setPen(pen);

    group = new CustomItemGroup();
    group->setFlag(QGraphicsItem::ItemIsSelectable);
    group->setFlag(QGraphicsItem::ItemIsMovable);

    qreal nodeRadius = CONFIG::NODE_RADIUS; // 节点半径
    qreal innerRadius = m_radius - nodeRadius - CONFIG::SHARD_BORDER; // 减去节点半径和边缘间距

    // 随机布局节点
    QRandomGenerator *generator = QRandomGenerator::global();
    qreal r = innerRadius * std::sqrt(generator->bounded(1.0));
    qreal angle = 2 * M_PI * generator->bounded(1.0);

    qreal node_center_x = rect().center().x() + r * std::cos(angle);
    qreal node_center_y = rect().center().y() + r * std::sin(angle);
    Node* node = new Node(0, 0, nodeRadius, QColor(CONFIG::MAINNODE_COLOR));
    node->setShard(this);

    group->addToGroup(node);
    node->setPos(node_center_x - nodeRadius, node_center_y - nodeRadius);
    nodes.append(node);
    m_mainNode = node;

    // 节点类的坐标是左上角，所以转换得有点扭曲。。。
    while(nodes.size() < nnm) {
        qreal r = innerRadius * std::sqrt(generator->bounded(1.0));
        qreal angle = 2 * M_PI * generator->bounded(1.0);
        qreal node_center_x = rect().center().x() + r * std::cos(angle);
        qreal node_center_y = rect().center().y() + r * std::sin(angle);
        if(isNonOverlapping(node_center_x, node_center_y, nodes, CONFIG::NODE_RADIUS * 4)){
            Node* node = new Node(0, 0, nodeRadius);
            group->addToGroup(node);
            node->setPos(node_center_x - nodeRadius, node_center_y - nodeRadius);
            nodes.append(node);
        }
    }

    group->addToGroup(this);
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
    QMutexLocker locker(&messages_mutex);

    for (int i = 0; i < messages.size(); ++i) {
        // 应对所有messages被清空的情况
        if(i >= messages.size()){
            return;
        }
        Message* msg = messages[i];
        QPointF end = msg->targetp;
        QPointF currentPos = msg->pos();
        QVector2D direction(end - currentPos);
        // 如果消息已经到达终点
        if (QVector2D(currentPos - end).length() < m_speed) {
            handleMessage(msg->target, msg);

            // handleMessage中可能删除所有消息
            // TODO: 这个解决方式很操蛋且不具有普遍性，考虑换一种方法。
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
        case MessageType::PROPOSE:
            node->setBrush(ColorMap::getColor(message->mtype));
            startPBFT(node);
            break;
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
        case MessageType::VERIFIED:
            qDebug() << "Message verified!";
            // node->setBrush(ColorMap::getColor(message->mtype));
            break;

        default:
            qDebug() << "Unknown message type!";
            break;
    }
}

// PBFT开始
void Shard::startPBFT(Node* node) {
    // 获取当前shard 的index
    LoggerManager::getInstance().addLog("分片 " + QString::number(idx) + " 开始PBFT共识!");
    broadcastMessage(node, MessageType::PRE_PREPARE);
}

// PBFT完成
void Shard::consensusDone(){
    LoggerManager::getInstance().addLog("分片 " + QString::number(idx) + " 共识结束，开始发送Reply!");

    if(replyAnimationTimer.isActive()){
        replyAnimationTimer.stop();
    }
    replyAnimationTimer.start(CONFIG::REPLY_ANIMATION_INTERVAL);

    // 删除消息
    for(int i = 0; i < messages.size(); i++){
        group->removeFromGroup(messages[i]);
        delete messages[i];
    }
    messages.clear();

    setBrush(Qt::green);
    for(int i = 0;i < nodes.size();i++){
        // nodes[i]->reset();
        nodes[i]->setVisible(false);
    }
}

void Shard::reply(){
    m_belongScene->shardReply(this);

    resetShard();
}

// 重置Shard为初始化状态
void Shard::resetShard(){
    // 删除消息
    for(int i = 0; i < messages.size(); i++){
        group->removeFromGroup(messages[i]);
        delete messages[i];
    }
    messages.clear();

    // 重置节点
    for(int i = 0; i < nodes.size(); i++){
        nodes[i]->reset();
    }

    // 关闭Timer
    if(replyAnimationTimer.isActive()){
        replyAnimationTimer.stop();
    }

    // 显示节点
    for(int i = 0;i < nodes.size();i++){
        nodes[i]->setVisible(true);
    }
    setBrush(Qt::transparent);
}

void Shard::resetSlot(){
    QMutexLocker locker(&messages_mutex);

    resetShard();
}

void Shard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // 调用基类的绘制函数绘制圆形
    QGraphicsEllipseItem::paint(painter, option, widget);

    // 设置文本颜色和样式
    painter->setPen(m_penColor);
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);

    // 在圆的下边绘制编号
    painter->drawText(rect().center().x() - 40, rect().bottom() + 20, QString("Shard %1").arg(idx));
}

// 加上编号的范围
QRectF Shard::boundingRect() const {
    return rect().adjusted(0, 0, 0, 20);
}

void Shard::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "Shard " << idx << " is clicked!";

    // update();
}
