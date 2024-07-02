#include "BlockChainDemo.hpp"
#include "Logger.hpp"


void BlockChainDemo::init(){
    // 使用默认鼠标缩放行为
    _sceneContext.registerDefaultCallbacks(_eventManager);
    
    // 设置日志级别
    Logger::getLogger().setLogLevel(Logger::LogLevel::INFO);
    Logger::getLogger().setOutputFile("log.txt");
    
    _blockChain.start();
}

int main(int argc, char** argv) {
    BlockChainDemo demo(title, sf::Vector2u(1920, 1080));
    demo.init();
    demo.run();
    return 0;
}