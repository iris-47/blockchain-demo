#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

// 区块链相关结构体

#include <SFML/Graphics.hpp>
#include <vector>

#include "EventManager.hpp"
#include "Struct.hpp"
#include "Logger.hpp"
#include "Shard.hpp"

/**
 * @class BlockChain
 * @brief 存储区块链相关的东西，你也可以叫他Updater,因为他负责更新所有动画状态
 */
class BlockChain{
    friend class Renderer; // 用于渲染器访问BlockChain
    friend class Controler; // 用于控制器访问BlockChain
private:
    std::vector<Node> nodes; // TODO:shards引入后，nodes失去了意义，考虑删除
    std::vector<Message> messages; // 代表分片外的消息，片内消息位于Shard结构体中
    std::vector<Shard> shards;     // 分片

    std::vector<Timer> timers; // 计时器, 先用着, 我知道丑

    cevm::CustomEventManager &_event_manager; // 所属事件管理器

    bool _running = false;
    float _speed = 1000.0f;

public:
    BlockChain(cevm::CustomEventManager &event_manager): 
        nodes(std::vector<Node>()), 
        _running(true), 
        _speed(1.0f), 
        _event_manager(event_manager){};

    void addNode(Node node){
        nodes.push_back(node);
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add node at (%f, %f)", node.getPosition().x, node.getPosition().y);
    }
    
    void addMessage(Message message){
        messages.push_back(message);
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add message from (%f, %f) to (%f, %f)", message.getStart().x, message.getStart().y, message.getEnd().x, message.getEnd().y);
    }

    void addShard(int NNum){
        shards.push_back(Shard(NNum, sf::Vector2f(0, 0), _event_manager));
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add shard with %d nodes", NNum);
    }

    void addShard(int NNum, sf::Vector2f center){
        shards.push_back(Shard(NNum, center, _event_manager));
        Logger::getLogger().log(Logger::LogLevel::INFO, "Add shard with %d nodes at (%f, %f)", NNum, center.x, center.y);
    }

    void invokePBFTs(std::vector<int> index){
        for(auto i : index){
            shards[i].startPBFT();
        }
    }

    // 临时使用
    void temp(){    
        for(int i = 0;i < 3;i++){
            Message message(shards[i].nodes[0], nodes[0], MessageType::PROPOSE);
            message.setColor(sf::Color::Green);
            addMessage(message);
        }
    }



    
    void update(sf::Time dt);
    void clear();
    void start();
    void pause();
};

#endif