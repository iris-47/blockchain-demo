#include "BlockChain.hpp"
#include <cmath>

BlockChain::BlockChain(): nodes(std::vector<Node>()), _running(true), _speed(1.0f){}

// 所有Node向右移动
void BlockChain::update(sf::Time dt){
    if(!_running) return;

    // 更新所有消息
    messages.erase(std::remove_if(messages.begin(), messages.end(), [&](Message& message){
        // 删除传递完成的消息
        return message.update(dt);
    }), messages.end());

    // 调用shard更新
    for(auto& shard : shards){
        shard.update(dt);
    }
}

void BlockChain::start(){
    addNode(Node(0, 0, 10, sf::Color::White));
    addNode(Node(100, 100, 10, sf::Color::Red));
    addNode(Node(200, 200, 10, sf::Color::Green));
    addNode(Node(300, 300, 10, sf::Color::Blue));

    addMessage(Message(nodes[0], nodes[1]));
    addMessage(Message(nodes[1], nodes[2]));
    addMessage(Message(nodes[2], nodes[3]));

    addShard(10, sf::Vector2f(500, 500));
    addShard(6 , sf::Vector2f(700, 700));

    shards[0].addMessage(0, 1);
    shards[0].addMessage(1, 2);

    _running = true;
}

void BlockChain::pause(){
    _running = false;
}