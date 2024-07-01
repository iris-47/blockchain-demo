#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include "Node.hpp"

// 发里福笑的粒子效果
struct Particle {
    sf::Vector2f velocity;
    sf::Vertex vertex;
    sf::Time lifetime;
};

// 消息类   
class Message { 
    friend class BlockChain;

private:
    float _progress;
    sf::Vector2f _start;
    sf::Vector2f _end;

public:
    Message(Node& from, Node& to):
        _progress(0.0f),
        _start(from.getPosition()),
        _end(to.getPosition()){}

    sf::Vector2f getStart(){return _start;}
    sf::Vector2f getEnd(){return _end;}
    float getProgress(){return _progress;}

    bool update(sf::Time);

    // 粒子效果相关
    // bool useParticleEffect = true; 
    // std::vector<Particle> particles;
    // int particleCount = 0;
    // const int maxParticleCount = 100;

    // void disableParticleEffect(){useParticleEffect = false;}
};


#endif