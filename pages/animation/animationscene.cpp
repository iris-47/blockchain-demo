#include "animationscene.h"
#include "utils/loggermanager.h""

#include <QVector2D>
#include <QDebug>

AnimationScene::AnimationScene(QObject *parent)
    : QGraphicsScene{parent}, timer{new QTimer{this}}, m_running{false}
{
    connect(timer, &QTimer::timeout, this, &AnimationScene::updateScene);
    m_speed = CONFIG::MESSAGE_SPEED;
    setConsensus();
}

void AnimationScene::initScene(){
    // 颜色#F70938

    addShard(0, 0, 8, QColor(CONFIG::SHARD_COLOR));

    addShard(400, 250, 10);
    addShard(-400, 200, 6);
    addShard(240, 100, 6);
    addShard(270, -200, 10);
    addShard(-400, -200, 8);
}

void AnimationScene::updateScene(){
    for(int i = 0;i < shards.size();i++){
        shards[i]->updateMessages();
    }

    updateMessages();
}

void AnimationScene::resetScene(){
    QMutexLocker locker(&messages_mutex);
    for(int i = 0;i < shards.size();i++){
        shards[i]->resetShard();
    }

    for(int i = 0;i < messages.size();i++){
        removeItem(messages[i]);
        delete messages[i];
    }

    messages.clear();

    // 关闭Timer
    if(timer->isActive()){
        timer->stop();
    }

    m_running = false;
}

void AnimationScene::restartTimer(){
    if(timer->isActive()){
        timer->stop();
        timer->start(1000/CONFIG::FRAME_RATE);
    }
}

void AnimationScene::startDemo(){
    if(m_running == false){
        QString logger_msg = "开始演示，发送消息从分片" + QString::number(m_startShardIndex) + "到分片";

        resetScene();
        for(int i = 0;i < m_endShardIndex.size();i++){
            // TODO: 解决分片数量设置问题
            if(m_endShardIndex[i] > shards.size()){
                qDebug() << "Invalid end shard index!";
                continue;
            }
            sendMessage(shards[m_startShardIndex]->nodes[0], shards[m_endShardIndex[i]], MessageType::PROPOSE);


            logger_msg += QString::number(m_endShardIndex[i]);
            if(i != m_endShardIndex.size() - 1){
                logger_msg += "、";
            }
        }
        LoggerManager::getInstance().addLog(logger_msg);
        m_running = true;
    }

    if(timer->isActive()){
        timer->stop();
    }else{
        timer->start(1000/CONFIG::FRAME_RATE);
    }
}

void AnimationScene::addNode(qreal x, qreal y, qreal r, QColor color)
{
    Node *node = new Node{0, 0, r, color};
    addItem(node);
    node->setPos(x, y);
    nodes.append(node);
}

void AnimationScene::sendMessage(Node *from, Node *to, MessageType mtype){
    // pos()获取的是Scnene坐标系坐标，boundingRect()获取的是Item的坐标系坐标，坑
    QPointF from_p = from->pos() + from->boundingRect().center();

    Message *message = new Message(from, to);

    addItem(message);
    message->setPos(from_p);
    messages.append(message);
}

// TODO 用group表示shard实在是太蠢了 我受不了这些坐标转换了。。。
// from shard表示从shard的主节点发送
void AnimationScene::sendMessage(Shard *from, Node *to, MessageType mtype){
    Message *message = new Message(CONFIG::MESSAGE_SIZE, mtype);
    // 将shard group坐标转化为scene坐标
    message->target = to;
    message->targetp = to->pos() + to->group()->pos();

    addItem(message);
    Node* start = from->m_mainNode;
    QPointF from_p = start->pos() + start->group()->pos();
    message->setPos(from_p);
    messages.append(message);
}

// 发送到shard表示发送到shard的主节点
void AnimationScene::sendMessage(Node *from, Shard *to, MessageType mtype){
    Message *message = new Message(CONFIG::MESSAGE_SIZE, mtype);
    // 将shard group坐标转化为scene坐标
    Node* target = to->m_mainNode;
    message->targetp = target->pos() + target->group()->pos();
    message->target = target;

    addItem(message);
    QPointF from_p = from->pos() + from->group()->pos();
    message->setPos(from_p);
    messages.append(message);
}

void AnimationScene::shardReply(Shard* shard){
    sendMessage(shard, shards[0]->m_mainNode, MessageType::VERIFIED);
}

void AnimationScene::updateMessages() {
    // 为 messages添加互斥锁
    QMutexLocker locker(&messages_mutex);

    for (int i = 0; i < messages.size(); ++i) {
        QPointF end = messages[i]->targetp;
        QPointF currentPos = messages[i]->pos();
        QVector2D direction(end - currentPos);
        // 如果消息已经到达终点
        if (QVector2D(currentPos - end).length() < m_speed) {
            // target所属的分片触发消息处理
            // TODO: 这也太他娘的丑了。。。
            Node* node = messages[i]->target;
            node->getShard()->handleMessage(node, messages[i]);

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
    Shard *shard = new Shard{0, 0, nnm, shards.size(), color};
    shard->m_belongScene = this;
    shards.append(shard);

    addItem(shard->group); // 作为整体添加
    shard->group->setPos(x, y);
}

void AnimationScene::setSpeed(){
    m_speed = CONFIG::MESSAGE_SPEED;
    for(int i = 0;i < shards.size();i++){
        shards[i]->setSpeed(CONFIG::INNER_MESSAGE_SPEED);
    }
}

void AnimationScene::setConsensus(){
    m_startShardIndex = CONFIG::START_INDEX;
    m_endShardIndex = CONFIG::END_INDEX;
}
