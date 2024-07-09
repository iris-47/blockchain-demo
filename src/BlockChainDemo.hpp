#include "Application.hpp"
#include "BlockChain.hpp"
#include "Renderer.hpp"
#include "Controler.hpp"

const char* title = "BlockChain Demo";
/**
 * @class BlockChainDemo
 * @brief 区块链演示程序, 继承自Application, 用于定义窗口与鼠标键盘事件, 并负责调用区块链更新和渲染器
 */
class BlockChainDemo : public Application {
private:
    BlockChain _blockChain; // 区块链结构，用于定义并更新节点和消息
    Renderer _renderer; // 渲染器，用于渲染节点和消息
    Controler _controler; // 控制器，用于控制区块链的暂停和继续等

    cevm::CustomEventManager _evm; // 自定义事件管理器(用于共识消息处理，计时器等)

public:
    BlockChainDemo(const std::string& window_name, sf::Vector2u window_size, uint32_t window_style = sf::Style::Default) :
        Application(window_name, window_size, window_style),
        _renderer(_sceneContext, _blockChain),
        _blockChain(_evm), 
        _controler(_blockChain, _renderer, *this){
        // setFont(Utils::fetch("consola.ttf").c_str(), 32.0f, true);
    }

    void init();
protected:
    void update(sf::Time dt) override {
        _blockChain.update(dt);
    }

    void render() override{
        _renderer.render();
        _controler.render();
    }
};