#include "Application.hpp"
#include <string>
#include <imgui.h>
#include <imgui-SFML.h>

Application::Application(const std::string& window_name, sf::Vector2u window_size, uint32_t style) :
    _window(sf::VideoMode(window_size.x, window_size.y), window_name, style),
    _eventManager(_window),
    _sceneContext(_window),
    _VSync(true) {

    _window.setFramerateLimit(60);
    _window.setVerticalSyncEnabled(_VSync);
    ImGui::SFML::Init(_window);
}

Application::~Application() {
    ImGui::SFML::Shutdown();
}

void Application::setFont(const char* filepath, float font_pixels, bool enable_chinese) {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(filepath, font_pixels, nullptr, enable_chinese ? io.Fonts->GetGlyphRangesChineseSimplifiedCommon() : 0);
    ImGui::SFML::UpdateFontTexture();
}

void Application::run() {
    sf::Clock clock;
    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (!ImGui::GetIO().WantCaptureMouse ||
                event.type == sf::Event::KeyPressed ||
                event.type == sf::Event::KeyReleased) {
                _eventManager.handleEvent(event);
            }

            ImGui::SFML::ProcessEvent(_window, event);
        }

        sf::Time dt = clock.restart();
        // if (dt > sf::seconds(0.02f)) {
        //     Logger::getLogger().log(Logger::LogLevel::DEBUG, "Frame time too long: " + std::to_string(dt.asSeconds()) + "s");
        //     continue;
        // }

        update(dt);
        ImGui::SFML::Update(_window, dt);

        _sceneContext.clear();
        render();
        ImGui::SFML::Render(_window);
        _sceneContext.display();
    }
}