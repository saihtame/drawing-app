#pragma once
#include "domain/documents/document.hpp"
#include "imgui.h"


namespace Presentation {

class CanvasPanel final {
public: // ctors
    CanvasPanel(std::weak_ptr<Domain::Documents::Document> document);
    ~CanvasPanel() = default;

public:
    [[nodiscard]] bool render();
    const std::string& get_panel_name() const;

private:
    std::weak_ptr<Domain::Documents::Document> document;
    std::string panel_name = "";
    bool open = true;

private: // Constants
    // ImGui window flags
    int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoMouseInputs;
};

}
