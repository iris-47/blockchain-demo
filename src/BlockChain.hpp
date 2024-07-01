#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

// 区块链相关结构体

#include <SFML/Graphics.hpp>
#include <vector>

#include "Node.hpp"
#include "Logger.hpp"
#include "Message.hpp"

class BlockChain{
    friend class Renderer; // 用于渲染器访问BlockChain
private:
    std::vector<Node> nodes;
    std::vector<Message> messages;

    bool _running = false;
    float _speed = 1000.0f;

public:
    BlockChain();

    void addNode(Node node){
        nodes.push_back(node);
        Logger::getLogger().log(Logger::LogLevel::DEBUG, "Add node at (%f, %f)", node.getPosition().x, node.getPosition().y);
    }
    
    void addMessage(Message message){
        messages.push_back(message);
        Logger::getLogger().log(Logger::LogLevel::DEBUG, "Add message from (%f, %f) to (%f, %f)", message.getStart().x, message.getStart().y, message.getEnd().x, message.getEnd().y);
    }
    
    void update(sf::Time dt);
    void start();
    void pause();
};

#endif