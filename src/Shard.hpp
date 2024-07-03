#ifndef SHARD_HPP
#define SHARD_HPP

#include <SFML/Graphics.hpp>
#include "Struct.hpp"
#include "config.h"
// 区块链分片
class Shard{
    friend class Renderer;
private:
    std::vector<Node> nodes;
    std::vector<Message> messages;
    
    sf::Vector2f _center; // 这些节点的中心点
    float _radius; // 分片绘制半径

public:
    Shard(int NNum, sf::Vector2f center);


    void addMessage(int from, int to, MessageType type = MessageType::DEFAULT);
    void broadcastMessage(int from, MessageType type = MessageType::DEFAULT);

    void PBFT(); // PBFT共识算法演示
    
    void update(sf::Time dt); // 更新分片状态，主要是消息的传递
};

#endif