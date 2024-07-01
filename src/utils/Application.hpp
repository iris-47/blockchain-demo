#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "EventManager.hpp"
#include "SceneContext.hpp"
#include "Logger.hpp"

class Application {
protected:
    sf::RenderWindow _window; // 窗口
    EventManager _eventManager; // 事件管理器
    SceneContext _sceneContext; // 场景上下文

    bool _VSync; // 是否开启垂直同步

    virtual void update(sf::Time dt) = 0; // 更新
    virtual void render() = 0; // 渲染

public:
    Application(const std::string& window_name, sf::Vector2u window_size, uint32_t style = sf::Style::Default);
    ~Application();

    bool getVSync() const { return _VSync; }
    void setVSync(bool VSync) { _VSync = VSync; _window.setVerticalSyncEnabled(VSync); }
    
    void setFont(const char *filepath, float font_pixels, bool enable_chinese = false);
    
    void run();
};

#endif