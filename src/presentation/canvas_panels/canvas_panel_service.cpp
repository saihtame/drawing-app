#include "canvas_panel_service.hpp"
#include <iostream>


namespace Presentation::CanvasPanels {

CanvasPanelService::CanvasPanelService(Application::Documents::DocumentService& doc_service)
 : doc_service(doc_service) {
}

void CanvasPanelService::add_panel(const std::shared_ptr<Domain::Documents::Document>& document) {
    if (contains(document)) {
        std::cerr << "CanvasPanelService cannot add panel for document. Already has one." << std::endl;
        return;
    }
    panels[document] = std::make_shared<CanvasPanel>(doc_service, document);

    std::cout << "CanvasPanelService added panel for document." << std::endl;
}

void CanvasPanelService::remove_panel(const std::shared_ptr<Domain::Documents::Document>& document) {
    auto it = panels.find(document);
    if (it == panels.end()) {
        std::cerr << "CanvasPanelService cannot remove panel for document. No panel to remove." << std::endl;
        return;
    }
    panels.erase(it);

    std::cout << "CanvasPanelService removed panel for document." << std::endl;
}

bool CanvasPanelService::contains(const std::shared_ptr<Domain::Documents::Document>& document) const {
    return panels.contains(document);
}

std::weak_ptr<CanvasPanel> CanvasPanelService::get_canvas_panel(const std::shared_ptr<Domain::Documents::Document>& document) const {
    auto it = panels.find(document);
    if (it == panels.end()) {
        return {};
    }
    return it->second;
}

const std::unordered_map<std::shared_ptr<Domain::Documents::Document>, std::shared_ptr<CanvasPanel>>& CanvasPanelService::get_panels() const {
    return panels;
}

}