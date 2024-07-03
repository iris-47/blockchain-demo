#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

// 区块链相关结构体

#include <SFML/Graphics.hpp>
#include <vector>

#include "Struct.hpp"
#include "Logger.hpp"
#include "Shard.hpp"

/**
 * @class BlockChain
 * @brief 存储区块链相关的东西，你也可以叫他Updater,因为他负责更新所有动画状态
 */
class BlockChain{
    friend class Renderer; // 用于渲染器访问BlockChain
private:
    std::vector<Node> nodes; // TODO:shards引入后，nodes失去了意义，考虑删除
    std::vector<Message> messages; // 代表分片外的消息，片内消息位于Shard结构体中
    std::vector<Shard> shards;     // 分片

    bool _running = false;
    float _speed = 1000.0f;

public:
    BlockChain();

    void addNode(Node node){
        nodes.push_back(node);
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add node at (%f, %f)", node.getPosition().x, node.getPosition().y);
    }
    
    void addMessage(Message message){
        messages.push_back(message);
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add message from (%f, %f) to (%f, %f)", message.getStart().x, message.getStart().y, message.getEnd().x, message.getEnd().y);
    }

    void addShard(int NNum){
        shards.push_back(Shard(NNum, sf::Vector2f(0, 0)));
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add shard with %d nodes", NNum);
    }

    void addShard(int NNum, sf::Vector2f center){
        shards.push_back(Shard(NNum, center));
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add shard with %d nodes at (%f, %f)", NNum, center.x, center.y);
    }
    
    void update(sf::Time dt);
    void start();
    void pause();
};

#endif