#pragma once
#include "application/documents/document_service.hpp"
#include "domain/documents/document.hpp"
#include "imgui.h"


namespace Presentation {

class CanvasPanel final {
public: // ctors
    CanvasPanel(Application::Documents::DocumentService& doc_service, std::weak_ptr<Domain::Documents::Document> document);
    ~CanvasPanel();

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

public:
    [[nodiscard]] bool render();
    const std::string& get_panel_name() const;
    bool is_visible() const { return visible; }
    glm::vec2 get_render_position() const { return render_position; }

private:
    std::weak_ptr<Domain::Documents::Document> document;
    std::string panel_name = "";
    int panel_no;
    bool open = true;
    bool visible = false;
    glm::vec2 render_position {0.0f, 0.0f};

private: // Constants
    // ImGui window flags
    int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoMouseInputs;
};

}
