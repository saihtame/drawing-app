#include "canvas_panel_service.hpp"
#include <iostream>


namespace Presentation::CanvasPanels {

CanvasPanelService::CanvasPanelService(Application::Documents::DocumentService& doc_service)
 : doc_service(doc_service) {
}

void CanvasPanelService::add_panel(const UUIDv4::UUID& uuid, const std::shared_ptr<Domain::Documents::Document>& document) {
    if (contains(uuid)) {
        std::cerr << "CanvasPanelService cannot add panel for document. Already has one." << std::endl;
        return;
    }
    panels[uuid] = std::make_shared<CanvasPanel>(doc_service, document);

    std::cout << "CanvasPanelService added panel for document." << std::endl;
}

void CanvasPanelService::remove_panel(const UUIDv4::UUID& uuid) {
    auto it = panels.find(uuid);
    if (it == panels.end()) {
        std::cerr << "CanvasPanelService cannot remove panel for document. No panel to remove." << std::endl;
        return;
    }
    panels.erase(it);

    std::cout << "CanvasPanelService removed panel for document." << std::endl;
}

bool CanvasPanelService::contains(const UUIDv4::UUID& uuid) const {
    return panels.contains(uuid);
}

std::weak_ptr<CanvasPanel> CanvasPanelService::get_canvas_panel(const UUIDv4::UUID& uuid) const {
    auto it = panels.find(uuid);
    if (it == panels.end()) {
        return {};
    }
    return it->second;
}

const std::unordered_map<UUIDv4::UUID, std::shared_ptr<CanvasPanel>>& CanvasPanelService::get_panels() const {
    return panels;
}

}