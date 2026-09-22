#pragma once
#include "application/documents/document_service.hpp"
#include "domain/documents/document.hpp"
#include "imgui.h"


namespace Presentation::CanvasPanels {

class CanvasPanel final {
public: // ctors
    CanvasPanel(Application::Documents::DocumentService& doc_service, const std::shared_ptr<Domain::Documents::Document>& document);
    ~CanvasPanel();

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

public: // Accessors
    [[nodiscard]] bool render();
    const std::string& get_panel_name() const;
    bool is_visible() const { return visible; }
    ImVec2 get_size() const { return size; }
    ImVec2 get_pos() const { return pos; }

public:
    void request_fit_to_screen() { fit_to_screen_pending = true; }

private:
    std::weak_ptr<Domain::Documents::Document> document;
    std::string panel_name = "";
    int panel_no;
    bool open = true;
    bool visible = false;
    ImVec2 size;
    ImVec2 pos;

private: // Action flags
    bool fit_to_screen_pending = false;

private: // Helpers
    inline void fit_to_screen();

private: // Constants
    // ImGui window flags
    int flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoMouseInputs;
};

}
