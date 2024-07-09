#include "BlockChain.hpp"
#include <cmath>


void BlockChain::update(sf::Time dt){
    if(!_running) return;

    // 更新Timer, 由于Timer调用的callback中会新增timers, 因此使用remove_if()的方式会导致错误，还是回归朴实吧
    // 那remove_if()还有啥优势？？？
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
    clear();
    addNode(Node(800, 200, 20, sf::Color::Cyan));

    addShard(6, sf::Vector2f(200, 500));
    addShard(6 , sf::Vector2f(800, 700));
    addShard(6 , sf::Vector2f(554, 864));
    addShard(6 , sf::Vector2f(1078, 753));
    addShard(6 , sf::Vector2f(922, 386));
    addShard(6 , sf::Vector2f(75, 125));

    addMessage(Message(nodes[0], shards[0].nodes[0], MessageType::PROPOSE));
    addMessage(Message(nodes[0], shards[1].nodes[0], MessageType::PROPOSE));
    addMessage(Message(nodes[0], shards[2].nodes[0], MessageType::PROPOSE));

    std::vector<int> shards_to_invoke;
    float interval = 1.0f / CONFIG::MESSAGE_SPEED + CONFIG::MESSAGE_GAP;

    shards_to_invoke.push_back(0);
    shards_to_invoke.push_back(1);
    shards_to_invoke.push_back(2);
    
    timers.emplace_back(interval, std::bind(&BlockChain::invokePBFTs, this, shards_to_invoke));
    
    timers.emplace_back(interval * 4, std::bind(&BlockChain::temp, this));

    _running = true;
}

void BlockChain::clear(){
    nodes.clear();
    messages.clear();
    shards.clear();
    timers.clear();
}

void BlockChain::pause(){
    if(_running){
        _running = false;
    }else{
        _running = true;
    }
}