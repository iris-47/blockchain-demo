#pragma once

// 渲染器

#include <SFML/Graphics.hpp>

#include "SceneContext.hpp"
#include "BlockChain.hpp"

class Renderer{
private:
    SceneContext& _scene;
    BlockChain& _blockChain;

    void renderNodes() const;
    void renderMessages() const;

public:
    Renderer(SceneContext& scene, BlockChain& blockChain);

    void render() const; // 渲染 
};