#include "Controler.hpp"
#include <imgui.h>

Controler::Controler(BlockChain &blockChain, Renderer& renderer, Application& app) :
    _blockChain(blockChain), _renderer(renderer), _app(app) {}

void Controler::render() {
    if (ImGui::Begin("Config")) {
        if (_blockChain._running) {
            if (ImGui::Button("Pause")) {
                pause();
            }
        } else {
            if (ImGui::Button("Run")) {
                pause();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Restart")) {
            _blockChain.start();
        }

        ImGui::SameLine();
        bool vsync = _app.getVSync();
        if (ImGui::Checkbox("vsync", &vsync)) {
            _app.setVSync(vsync);
        }

        ImGui::SliderFloat("speed", &_blockChain._speed, 0.0f, 3.0f);

        ImGui::SameLine();
        if (ImGui::Button("AddShard")) {
            
        }
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Shards Info", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < _blockChain.shards.size(); ++i) {
                auto& shard = _blockChain.shards[i];
                std::string index = std::to_string(i + 1);
                if (ImGui::TreeNode(("shard #" + index).c_str())) {
                    if (ImGui::SliderFloat(("x##" + index).c_str(), &shard._center.x, 0, 1920))
                        _blockChain.start();
                    if (ImGui::SliderFloat(("y##" + index).c_str(), &shard._center.y, 0, 1080))
                        _blockChain.start();
                    if (ImGui::DragFloat(("radius##" + index).c_str(), &shard._radius, 10, 200))
                        _blockChain.start();
                    auto f4color = ImGui::ColorConvertU32ToFloat4(*(ImU32*)(&shard._color.r));
                    if (ImGui::ColorEdit4(("color##" + index).c_str(), (float*)(&f4color))) {
                        *(ImU32*)(&shard._color.r) = ImGui::ColorConvertFloat4ToU32(f4color);
                    }
                    ImGui::TreePop();
                }
            }
        }
    }
    ImGui::End();
}

void Controler::pause() {
    _blockChain.pause();
}