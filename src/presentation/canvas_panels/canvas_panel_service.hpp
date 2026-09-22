#pragma once
#include "application/documents/document_service.hpp"
#include "canvas_panel.hpp"
#include <memory>
#include <unordered_map>


namespace Presentation::CanvasPanels {

class CanvasPanelService {
public: // Ctors
    CanvasPanelService(Application::Documents::DocumentService& doc_service);

public: // Panel management
    void add_panel(const UUIDv4::UUID& uuid, const std::shared_ptr<Domain::Documents::Document>& document);
    void remove_panel(const UUIDv4::UUID& uuid);
    [[nodiscard]] bool contains(const UUIDv4::UUID& uuid) const;
    void render();
    std::weak_ptr<CanvasPanel> get_visible_panel() const { return visible_panel; }

public: // Accessors
    [[nodiscard]] std::weak_ptr<CanvasPanel> get_canvas_panel(const UUIDv4::UUID& uuid) const;
    [[nodiscard]] const std::unordered_map<UUIDv4::UUID, std::shared_ptr<CanvasPanel>>& get_panels() const;

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

private: // Data
    std::unordered_map<UUIDv4::UUID, std::shared_ptr<CanvasPanel>> panels;
    std::weak_ptr<CanvasPanel> visible_panel;
};

}
