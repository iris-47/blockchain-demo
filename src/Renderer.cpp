#include "Renderer.hpp"
#include "Logger.hpp"

Renderer::Renderer(SceneContext& scene, BlockChain& blockChain): _scene(scene), _blockChain(blockChain){}

void Renderer::render() const{
    renderNodes();
    renderMessages();
    renderShards();
}


void Renderer::renderNode(Node& node) const{
    sf::CircleShape circle(node.getRadius());
    circle.setFillColor(node.getColor());
    circle.setPosition(node.getPosition() - sf::Vector2f(node.getRadius(), node.getRadius()));

    _scene.draw(circle);
}

void Renderer::renderNodes() const{
    for(auto& node: _blockChain.nodes){
        renderNode(node);
    }
}

// 渲染两点间消息的传递过程
void Renderer::renderMessages() const{
    for(auto &message: _blockChain.messages){
        // 绘制消息传递的进度点
        sf::CircleShape messageDot(5.f);
        messageDot.setFillColor(sf::Color::Yellow);

        sf::Vector2f messagePosition = message.getStart() + (message.getEnd() - message.getStart()) * message.getProgress();
        messageDot.setPosition(messagePosition - sf::Vector2f(messageDot.getRadius(), messageDot.getRadius()));

        _scene.draw(messageDot);

        // 绘制粒子效果
        // sf::VertexArray particleArray(sf::Points, message.particleCount);
        // for (int i = 0; i < message.particleCount; ++i) {
        //     particleArray[i] = message.particles[i].vertex;
        // }
        // _scene.draw(particleArray);
    }
}

void Renderer::renderShards() const{
    for(auto &shard: _blockChain.shards){
        // 绘制分片框
        sf::CircleShape circle(shard._radius);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(1.f);
        circle.setPosition(shard._center - sf::Vector2f(shard._radius, shard._radius));
        _scene.draw(circle);


        // 绘制分片的节点
        for(auto &node: shard.nodes){
            renderNode(node);
        }
        
        // 绘制分片内消息
        for(auto &message: shard.messages){
            // 绘制消息传递的进度点
            sf::CircleShape messageDot(5.f);
            messageDot.setFillColor(ColorMap::getColor(message._type));

            sf::Vector2f messagePosition = message.getStart() + (message.getEnd() - message.getStart()) * message.getProgress();
            messageDot.setPosition(messagePosition - sf::Vector2f(messageDot.getRadius(), messageDot.getRadius()));

            _scene.draw(messageDot);
            Logger::getLogger().log(Logger::LogLevel::INFO, "Render Intra-Shard message at (%f, %f)", messagePosition.x, messagePosition.y);
        }
    }
}