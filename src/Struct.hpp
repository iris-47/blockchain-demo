#ifndef NODE_HPP
#define NODE_HPP
#include <SFML/Graphics.hpp>
#include <functional>
#include "config.h"
#include "Logger.hpp"

// the node draw in the screen
class Node {
    friend class Renderer;
private:
    sf::Vector2f _position;
    float _radius;
    sf::Color _color; 

public:
    Node(float x, float y, float radius, sf::Color color):
        _position(x, y), _radius(radius), _color(color){}
    
    Node():
        _position(0, 0), _radius(0), _color(sf::Color::White){}

    void setX(float x){_position.x = x;}
    void setY(float y){_position.y = y;}
    void setRadius(float radius){_radius = radius;}
    void setColor(sf::Color color){_color = color;}

    sf::Vector2f getPosition(){return _position;}
    float getRadius(){return _radius;}

    sf::Color getColor(){return _color;}
};


// 发里福笑的粒子效果
struct Particle {
    sf::Vector2f velocity;
    sf::Vertex vertex;
    sf::Time lifetime;
};

// 消息类   
class Message { 
    friend class Renderer;

private:
    float _progress;
    sf::Vector2f _start;
    sf::Vector2f _end;

    MessageType _type;

    sf::Color _color;

public:
    Message(Node& from, Node& to, MessageType type = MessageType::DEFAULT):
        _progress(0.0f),
        _start(from.getPosition()),
        _end(to.getPosition()),
        _type(type){}

    void setColor(sf::Color color){_color = color;}

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


/// @brief 计时器类, 以秒为单位
class Timer{
public:
    Timer(): _interval(0.0f), _elapsed(0.0f), _active(false), _callback(nullptr){}
    Timer(float interval) : _interval(interval), _elapsed(0.0f), _active(false) {}

    // 注意这个构造函数对应active为true
    Timer(float interval, std::function<void()> callback) : _interval(interval), _elapsed(0.0f), _active(true), _callback(callback) {}

    // 清空计时器并开始计时
    void start(float interval){
        _elapsed = 0.0f;
        _interval = interval;
        _active = true;
    }

    void stop(){_active = false;}

    void update(sf::Time dt){if(_active) _elapsed += dt.asSeconds();}

    bool isExpired(){
        if(_elapsed >= _interval){
            return true;
        }
        return false;
    }

    void invokeCallback(){
        if(_callback){
            _callback();
        }
    }
    
private:
    std::function<void()> _callback;

    bool _active;

    float _interval;
    float _elapsed;
};


#endif