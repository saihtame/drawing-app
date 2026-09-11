#pragma once
#include "application/documents/document_service.hpp"
#include "domain/documents/document.hpp"
#include "imgui.h"


namespace Presentation::CanvasPanels {

class CanvasPanel final {
public: // ctors
    CanvasPanel(Application::Documents::DocumentService& doc_service, std::weak_ptr<Domain::Documents::Document> document);
    ~CanvasPanel();

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

public:
    [[nodiscard]] bool render();
    const std::string& get_panel_name() const;

private:
    std::weak_ptr<Domain::Documents::Document> document;
    std::string panel_name = "";
    int panel_no;
    bool open = true;

private: // Constants
    // ImGui window flags
    int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoMouseInputs;
};

}
