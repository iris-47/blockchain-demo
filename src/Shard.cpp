#include "struct.hpp"
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
    _radius = (std::sqrt(NNum) + 1) * RADIUS_RATE;

    int innerRadius = _radius * 0.8f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0, 1);

    // push leader node
    float r = innerRadius * std::sqrt(dis(gen));
    float theta = 2 * PI * dis(gen);
    float x = center.x + r * std::cos(theta);
    float y = center.y + r * std::sin(theta);
    nodes.push_back(Node(x, y, NODE_RADIUS,sf::Color::Yellow));

    while(nodes.size() < NNum){
        float r = innerRadius * std::sqrt(dis(gen));
        float theta = 2 * PI * dis(gen);
        float x = center.x + r * std::cos(theta);
        float y = center.y + r * std::sin(theta);
        Node node = Node(x, y, NODE_RADIUS,sf::Color::White);

        Logger::getLogger().log(Logger::LogLevel::INFO, "Add node at (%f, %f)", node.getPosition().x, node.getPosition().y);
        if(isNonOverlapping(node, nodes, NODE_RADIUS * 4)){
            nodes.push_back(node);
        }
    }

    // // 使用网格来布局节点
    // int gridSize = std::sqrt(NNum) + 1;
    // float spacing = (2 * innerRadius) / gridSize; // 网格间距
    // int count = 0;

    // for(int i = 0; i < gridSize; i++){
    //     for(int j = 0; j < gridSize; j++){
    //         Node node = Node(
    //             center.x + (i + 0.5f) * spacing - innerRadius,
    //             center.y + (j + 0.5f) * spacing - innerRadius,
    //             NODE_RADIUS,
    //             sf::Color::White
    //         );

    //         if(count == 0){
    //             node.setColor(sf::Color::Yellow);
    //         }
    //         nodes.push_back(node);

    //         count ++;
    //         if(count >= NNum){
    //             return;
    //         }
    //     }
    // }
}