#ifndef SHARD_HPP
#define SHARD_HPP

#include <SFML/Graphics.hpp>
#include "EventManager.hpp"
#include "Struct.hpp"
#include "config.h"
// 区块链分片
class Shard{
    // 我知道这么多friend丑，先用着
    friend class Renderer;
    friend class BlockChain;
    friend class Controler;
private:
    cevm::CustomEventManager &_event_manager; // 所属事件管理器
    std::vector<Node> nodes;
    std::vector<Message> messages;

    std::vector<Timer> timers; // 用于PBFT共识算法的计时器

    sf::Vector2f _center; // 这些节点的中心点
    float _radius; // 分片绘制半径
    sf::Color _color; // 分片颜色

public:
    Shard(int NNum, sf::Vector2f center, cevm::CustomEventManager &event_manager);

    void addMessage(int from, int to, MessageType type = MessageType::DEFAULT);
    void broadcastMessage(int from, MessageType type = MessageType::DEFAULT);

    void registerEvents(); // 注册事件
    void startPBFT(); // PBFT共识算法演示
    
    void handlePropose();
    void handlePrePrepare();
    void handlePrepare();
    void handleCommit();


    void update(sf::Time dt); // 更新分片状态，主要是消息的传递
};

#endif