#include "BlockChainDemo.hpp"
#include "Logger.hpp"
#include "config.h"


void BlockChainDemo::init(){
    // 设置字体
    setFont("C:\\Windows\\Fonts\\consola.ttf", 32.0f, true);

    // 使用默认鼠标缩放行为
    _sceneContext.registerDefaultCallbacks(_eventManager);
    
    // 设置日志级别
    Logger::getLogger().setLogLevel(Logger::LogLevel::ERROR);
    Logger::getLogger().setOutputFile("log.txt");
    
    _eventManager.addEventCallback(sf::Event::KeyPressed, sf::Keyboard::Key::Space, [this](const sf::Event& event){
        _controler.pause();
    });

    _blockChain.start();
}

int main(int argc, char** argv) {
    BlockChainDemo demo(title, sf::Vector2u(CONFIG::WINDOW_WIDTH, CONFIG::WINDOW_HEIGHT));
    demo.init();
    demo.run();
    return 0;
}