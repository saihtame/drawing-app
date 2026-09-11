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
    void add_panel(const std::shared_ptr<Domain::Documents::Document>& document);
    void remove_panel(const std::shared_ptr<Domain::Documents::Document>& document);
    [[nodiscard]] bool contains(const std::shared_ptr<Domain::Documents::Document>& document) const;

public: // Accessors
    [[nodiscard]] std::weak_ptr<CanvasPanel> get_canvas_panel(const std::shared_ptr<Domain::Documents::Document>& document) const;
    [[nodiscard]] const std::unordered_map<std::shared_ptr<Domain::Documents::Document>, std::shared_ptr<CanvasPanel>>& get_panels() const;

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

private: // Data
    std::unordered_map<std::shared_ptr<Domain::Documents::Document>, std::shared_ptr<CanvasPanel>> panels;
};

}