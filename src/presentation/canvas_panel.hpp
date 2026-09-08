#pragma once
#include "application/documents/document_canvas.hpp"
#include "imgui.h"
#include <memory>


namespace Presentation {

class CanvasPanel final {
public: // ctors
    CanvasPanel(std::weak_ptr<Application::Documents::DocumentCanvas> canvas);
    ~CanvasPanel() = default;

public:
    void render();
    const std::string& get_panel_name() const;

private:
    std::weak_ptr<Application::Documents::DocumentCanvas> canvas;
    std::string panel_name = "";
    bool open = true;

private: // Constants
    // ImGui window flags
    int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMouseInputs;
};

}
