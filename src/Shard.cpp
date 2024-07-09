#include "Shard.hpp"
#include "Struct.hpp"
#include "Logger.hpp"
#include "config.h"

#include <cmath>
#include <vector>
#include <random>

// 检测节点是否重叠
bool isNonOverlapping(Node& node, std::vector<Node>& nodes, float minDistance) {
    for (auto& existingNode : nodes) {
        float distance = std::sqrt(std::pow(node.getPosition().x - existingNode.getPosition().x, 2) + std::pow(node.getPosition().y - existingNode.getPosition().y, 2));
        if (distance < minDistance) {
            return false;
        }
    }
    return true;
}

Shard::Shard(int NNum, sf::Vector2f center, cevm::CustomEventManager &event_manager): _event_manager(event_manager){
    _center = center;
    _radius = (std::sqrt(NNum) + 1) * CONFIG::RADIUS_RATE;

    int innerRadius = _radius * 0.8f;

    // 随机布局节点
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0, 1);

    // push leader node
    float r = innerRadius * std::sqrt(dis(gen));
    float theta = 2 * PI * dis(gen);
    float x = center.x + r * std::cos(theta);
    float y = center.y + r * std::sin(theta);
    nodes.push_back(Node(x, y, CONFIG::NODE_RADIUS,sf::Color::Yellow));

    while(nodes.size() < NNum){
        float r = innerRadius * std::sqrt(dis(gen));
        float theta = 2 * PI * dis(gen);
        float x = center.x + r * std::cos(theta);
        float y = center.y + r * std::sin(theta);
        Node node = Node(x, y, CONFIG::NODE_RADIUS,sf::Color::White);

        Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Add node at (%f, %f)",  __LINE__, node.getPosition().x, node.getPosition().y);
        if(isNonOverlapping(node, nodes, CONFIG::NODE_RADIUS * 4)){
            nodes.push_back(node);
        }
    }
}

void Shard::addMessage(int from, int to, MessageType type){
    if(from < 0 || from >= nodes.size() || to < 0 || to >= nodes.size()){
        Logger::getLogger().log(Logger::LogLevel::ERROR, "%d:Invalid message from %d to %d", __LINE__, from, to);
        return;
    }

    messages.push_back(Message(nodes[from], nodes[to], type));
    Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Add Intra-Shard message from (%f, %f) to (%f, %f), now legnth:%d", __LINE__, nodes[from].getPosition().x, nodes[from].getPosition().y, nodes[to].getPosition().x, nodes[to].getPosition().y, messages.size());
}

void Shard::broadcastMessage(int from, MessageType type){
    if(from < 0 || from >= nodes.size()){
        Logger::getLogger().log(Logger::LogLevel::ERROR, "%d:Invalid index %d!", __LINE__, from);
        return;
    }

    for(int i = 0; i < nodes.size(); i++){
        if(i != from){
            messages.push_back(Message(nodes[from], nodes[i], type));
            Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Add Intra-Shard message from (%f, %f) to (%f, %f), now legnth:%d", __LINE__, nodes[from].getPosition().x, nodes[from].getPosition().y, nodes[i].getPosition().x, nodes[i].getPosition().y, messages.size());
        }
    }
}

void Shard::handlePropose(){
    Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Invoke PROPOSE", __LINE__);

    broadcastMessage(0, MessageType::PRE_PREPARE);
    
    float interval = 1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP;

    timers.emplace_back(interval, std::bind(&Shard::handlePrePrepare, this));
}

void Shard::handlePrePrepare(){
    Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Invoke PREPARE", __LINE__);

    for(int i = 1; i < nodes.size(); i++){
        broadcastMessage(i, MessageType::PREPARE);
    }

    float interval = 1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP;

    timers.emplace_back(interval, std::bind(&Shard::handlePrepare, this));
}

void Shard::handlePrepare(){
    Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Invoke COMMIT", __LINE__);

    for(int i = 1; i < nodes.size(); i++){
        broadcastMessage(i, MessageType::COMMIT);
    }

    float interval = 1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP;
    
    timers.emplace_back(interval, std::bind(&Shard::handleCommit, this));
}

void Shard::handleCommit(){
    // 回送消息给LL
}

// /// @brief 所有Shards一共仅调用一次！
// void Shard::registerEvents(){
//    // 注册PBFT事件（我承认愚蠢又不优雅，但我想不到更方便的方式）
//     _event_manager.registerEventHandler(cevm::EventType::PROPOSE, [&](cevm::Event event){
//         Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Invoke PROPOSE", __LINE__);

//         broadcastMessage(0, MessageType::PRE_PREPARE);
//         timer.start(1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP);
//         _event_manager.handleEvent(cevm::Event{cevm::EventType::PREPARE, "PREPARE"});
//     });

//     _event_manager.registerEventHandler(cevm::EventType::PREPARE, [&](cevm::Event event){
//         Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Invoke PREPARE", __LINE__);

//         for(int i = 1; i < nodes.size(); i++){
//             broadcastMessage(i, MessageType::PREPARE);
//         }
//         timer.start(1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP);
//         _event_manager.handleEvent(cevm::Event{cevm::EventType::COMMIT, "COMMIT"});
//     });

//     _event_manager.registerEventHandler(cevm::EventType::COMMIT, [&](cevm::Event event){
//         Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Invoke COMMIT", __LINE__);

//         for(int i = 1; i < nodes.size(); i++){
//             broadcastMessage(i, MessageType::COMMIT);
//         }
//         timer.start(1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP);
//     });
// }

void Shard::startPBFT(){
    Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Start PBFT", __LINE__);
    
    handlePropose();
    // _event_manager.handleEvent(cevm::Event{cevm::EventType::PROPOSE, "PROPOSE"});
}

void Shard::update(sf::Time dt){
    // 更新Timer, 由于Timer调用的callback中会新增timers, 因此使用remove_if()的方式会导致错误，还是回归朴实吧
    if(timers.size() > 0){
        for(int i = 0;i < timers.size();i++){
            timers[i].update(dt);
            if(timers[i].isExpired()){
                Logger::getLogger().log(Logger::LogLevel::INFO, "%d:Timer expired", __LINE__);
                timers[i].invokeCallback();
                timers.erase(timers.begin() + i);
                i--;
            }
        }
    }

    // 更新所有消息
    if(messages.size() > 0){
        messages.erase(std::remove_if(messages.begin(), messages.end(), [&](Message& message){
            // 删除传递完成的消息
            return message.update(dt);
        }), messages.end());
    }
}