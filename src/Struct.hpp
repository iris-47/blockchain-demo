#ifndef NODE_HPP
#define NODE_HPP
#include <SFML/Graphics.hpp>
#include "config.h"

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

public:
    Message(Node& from, Node& to, MessageType type = MessageType::DEFAULT):
        _progress(0.0f),
        _start(from.getPosition()),
        _end(to.getPosition()),
        _type(type){}

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