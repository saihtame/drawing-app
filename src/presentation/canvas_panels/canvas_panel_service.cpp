#include "canvas_panel_service.hpp"
#include <iostream>
#include <memory>


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

void CanvasPanelService::render() {
    visible_panel = std::weak_ptr<CanvasPanel>();
    // Render panels
    std::vector<UUIDv4::UUID> uuids_to_remove;
    for (const auto& [doc, panel]: panels) {
        bool should_remove = panel->render();
        if (should_remove) {
            uuids_to_remove.push_back(doc);
        } else if (panel->is_visible()) {
            visible_panel = panel;
        }
    }
    // Remove documents if they have been closed
    for (const auto& uuid : uuids_to_remove) {
        doc_service.close_document(uuid);
    }
}

}
