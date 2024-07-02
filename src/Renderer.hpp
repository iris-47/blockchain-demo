#pragma once

// 渲染器

#include <SFML/Graphics.hpp>

#include "SceneContext.hpp"
#include "BlockChain.hpp"

class Renderer{
private:
    SceneContext& _scene;
    BlockChain& _blockChain;

    void renderNode(Node& node) const;
    void renderNodes() const;   
    void renderMessages() const;
    void renderShards() const;

public:
    Renderer(SceneContext& scene, BlockChain& blockChain);

    void render() const; // 渲染 
};