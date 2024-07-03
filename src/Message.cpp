#include "Struct.hpp"
#include "config.h"

bool Message::update(sf::Time dt){
    // 更新粒子效果
    // if(useParticleEffect){
    //     particles.erase(std::remove_if(particles.begin(), particles.end(), [&](Particle& particle){
    //         particle.vertex.position += particle.velocity * dt.asSeconds();
    //         particle.vertex.color.a = (sf::Uint8)((particle.lifetime / sf::seconds(0.1f)) * 255);
    //         particle.lifetime -= dt;

    //         // 删除生命周期为0的粒子
    //         return particle.lifetime < sf::Time::Zero;
    //     }), particles.end());
    // }

    // 更新消息走的进度, 所有消息存在时间固定，距离越快速度越快
    _progress += dt.asSeconds() * CONFIG::MESSAGE_SPEED; 
    return _progress >= 1.0f;
}