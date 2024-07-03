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

Shard::Shard(int NNum, sf::Vector2f center){
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

        Logger::getLogger().log(Logger::LogLevel::INFO, "Add node at (%f, %f)", node.getPosition().x, node.getPosition().y);
        if(isNonOverlapping(node, nodes, CONFIG::NODE_RADIUS * 4)){
            nodes.push_back(node);
        }
    }
}

void Shard::addMessage(int from, int to){
    if(from < 0 || from >= nodes.size() || to < 0 || to >= nodes.size()){
        Logger::getLogger().log(Logger::LogLevel::ERROR, "Invalid message from %d to %d", from, to);
        return;
    }

    messages.push_back(Message(nodes[from], nodes[to]));
    Logger::getLogger().log(Logger::LogLevel::INFO, "Add Intra-Shard message from (%f, %f) to (%f, %f)", nodes[from].getPosition().x, nodes[from].getPosition().y, nodes[to].getPosition().x, nodes[to].getPosition().y);
}

void Shard::update(sf::Time dt){
    // 更新所有消息
    messages.erase(std::remove_if(messages.begin(), messages.end(), [&](Message& message){
        // 删除传递完成的消息
        return message.update(dt);
    }), messages.end());
}