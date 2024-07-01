#include "BlockChainDemo.hpp"
#include "Logger.hpp"


void BlockChainDemo::init(){
    // 设置日志级别
    Logger::getLogger().setLogLevel(Logger::LogLevel::DEBUG);
    Logger::getLogger().setOutputFile("log.txt");
    
    _blockChain.start();
}

int main(int argc, char** argv) {
    BlockChainDemo demo(title, sf::Vector2u(1080, 720));
    demo.init();
    demo.run();
    return 0;
}