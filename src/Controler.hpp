#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include "Application.hpp"
#include "Renderer.hpp"
#include "BlockChain.hpp"

// #include "Model.hpp"

class Controler {
    BlockChain &_blockChain;
    Renderer &_renderer;
    Application &_app;

public:
    Controler(BlockChain& blockChain, Renderer &renderer, Application &application);
    void render();

    void pause();
};

#endif