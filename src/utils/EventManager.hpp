#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "Logger.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

// EventManager
namespace evmn {
    // 用于Event字典的Key
    typedef struct{
        sf::Event::EventType type;
        sf::Mouse::Button button;
        sf::Keyboard::Key code;
    } EventKey;

    struct EventKeyHash {
        std::size_t operator()(const EventKey& k) const {
            return std::hash<int>()(k.type) ^ (std::hash<int>()(k.code) << 1);
        }
    };

    struct EventKeyEqual {
        bool operator()(const EventKey& lhs, const EventKey& rhs) const {
            return lhs.type == rhs.type && lhs.code == rhs.code;
        }
    };

    using EventCallback = std::function<void(const sf::Event& event)>;
    using EventCallbackMap = std::unordered_map<EventKey, EventCallback, EventKeyHash, EventKeyEqual>;
}


class EventManager {
private:
    sf::Window& _window; // 所属窗口
    evmn::EventCallbackMap _event_map; // 事件回调字典

public:
    EventManager(sf::RenderWindow& window) : _window(window) {}
    
    // 添加事件回调
    void addEventCallback(sf::Event::EventType type, evmn::EventCallback callback){
        evmn::EventKey key;
        key.type = type;
        key.button = sf::Mouse::Button::ButtonCount;
        key.code = sf::Keyboard::Key::Unknown;

        _event_map[key] = callback;
    }

    // 添加鼠标事件回调
    void addEventCallback(sf::Event::EventType type, sf::Mouse::Button button, evmn::EventCallback callback){
        evmn::EventKey key;
        key.type = type;
        key.button = button;
        key.code = sf::Keyboard::Key::Unknown;

        _event_map[key] = callback;
    }

    // 添加键盘事件回调
    void addEventCallback(sf::Event::EventType type, sf::Keyboard::Key code, evmn::EventCallback callback){
        evmn::EventKey key;
        key.type = type;
        key.button = sf::Mouse::Button::ButtonCount;
        key.code = code;

        _event_map[key] = callback;
    }

    // 处理所有事件
    void handleEvents(evmn::EventCallback fallback = nullptr) const{
        sf::Event event;
        while (_window.pollEvent(event)) {
            handleEvent(event, fallback);
        }
    }
    
    // 处理单个事件
    void handleEvent(const sf::Event& event, evmn::EventCallback fallback = nullptr) const{
        evmn::EventKey key;
        key.type = event.type;
        
        if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased){
            key.button = event.mouseButton.button;
        }else{
            key.button = sf::Mouse::Button::ButtonCount;
        }
            
        if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
            key.code = event.key.code;
        }else{
            key.code = sf::Keyboard::Key::Unknown;
        }
        
        Logger::getLogger().log(Logger::LogLevel::DEBUG, "Processing event: " + std::to_string(key.type) + " " + std::to_string(key.button) + " " + std::to_string(key.code) + "\n");

        auto it = _event_map.find(key);

        if(it != _event_map.end()){
            it->second(event);
        } else if(fallback != nullptr){
            fallback(event);
        }
    }
};

#endif