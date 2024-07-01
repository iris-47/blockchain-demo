#ifndef NODE_HPP
#define NODE_HPP
#include <SFML/Graphics.hpp>

// the node draw in the screen
class Node {
    friend class BlockChain;
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

#endif