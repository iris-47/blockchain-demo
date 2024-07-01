#ifndef SCENECONTEXT_HPP
#define SCENECONTEXT_HPP

#include <SFML/Graphics.hpp>
#include "EventManager.hpp"
#include "Logger.hpp"
#include <iostream>

class SceneContext {
private:
    sf::RenderWindow& _window; // 所属窗口
    sf::View _view;            // 摄像机

    sf::Vector2f _oldPos;   // 上一次鼠标位置，用于鼠标拖动

    bool _update = false;   // 本帧是否需要更新_view
    bool _clicking = false; // 鼠标是否按下

public:
    SceneContext(sf::RenderWindow& window):
        _window(window),
        _view(window.getDefaultView()){}

    void draw(const sf::Drawable& drawable, const sf::RenderStates& render_states = {}){
        if(_update){
            _window.setView(_view);
        }
        _window.draw(drawable, render_states);
    }

    void clear(const sf::Color& color = sf::Color::Black){
        _window.clear(color);
    }

    void display(){
        _window.display();
    }

    void registerDefaultCallbacks(EventManager& eventManager) {
        // 关闭窗口
        eventManager.addEventCallback(sf::Event::Closed, [&](const sf::Event& e){_window.close();});
        eventManager.addEventCallback(sf::Event::KeyPressed, sf::Keyboard::Key::Escape, [&](const sf::Event& e){_window.close();});

        // 鼠标拖动背景移动
        eventManager.addEventCallback(sf::Event::MouseButtonPressed, sf::Mouse::Left, [&](const sf::Event& e){click(e.mouseButton.x, e.mouseButton.y);Logger::getLogger().log(Logger::LogLevel::DEBUG, "mouse click detected!");});
        eventManager.addEventCallback(sf::Event::MouseButtonReleased, sf::Mouse::Left, [&](const sf::Event& e){unclick();Logger::getLogger().log(Logger::LogLevel::DEBUG, "mouse unclick detected!");});
        eventManager.addEventCallback(sf::Event::MouseMoved, [&](const sf::Event& e){move(e.mouseMove.x, e.mouseMove.y);});
    
        // 鼠标滚轮缩放
        eventManager.addEventCallback(sf::Event::MouseWheelScrolled, [&](const sf::Event& e){zoom(e.mouseWheelScroll.delta);});
    }

    void click(int x, int y){
        _oldPos = sf::Vector2f((float)x, (float)y);
        _clicking = true;
    }

    void unclick(){
        _clicking = false;
    }

    void move(int x, int y){
        if(_clicking){
            sf::Vector2f newPos = sf::Vector2f((float)x, (float)y);
            sf::Vector2f delta = _oldPos - newPos;
            _view.move(delta);
            _oldPos = newPos;
            _update = true;
        }
    }

    void zoom(float delta){
        _view.zoom(delta > 0 ? 1 / 1.2f : 1.2f);
        _update = true;
    }
};

#endif