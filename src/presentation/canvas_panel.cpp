#include "canvas_panel.hpp"
#include "imgui.h"
#include <iostream>
#include <limits>
#include <unordered_set>


namespace Presentation {


// Each panel just uses this no for its ImGui window name and increments the value.
// This ensures no two panels have the same name.
std::unordered_set<int> names_taken;

static int get_canvas_number() {
    for (int i = 0; i < std::numeric_limits<int>::max(); i++)  {
        if (!names_taken.contains(i)) {
            names_taken.insert(i);
            return i;
        }
    }
    return -1;
}
static void return_canvas_number(int number) {
    auto it = names_taken.find(number);

    if (it != names_taken.end()) {
        names_taken.erase(it);
    }
}

CanvasPanel::CanvasPanel(Application::Documents::DocumentService& doc_service, std::weak_ptr<Domain::Documents::Document> document)
 : doc_service(doc_service), document(document) {
    panel_no = get_canvas_number();
    panel_name = "Image " + std::to_string(panel_no);
}

CanvasPanel::~CanvasPanel() {
    return_canvas_number(panel_no);
}

bool CanvasPanel::render() {
    if (document.expired())
        return false;

    visible = ImGui::Begin(panel_name.c_str(), &open, flags);
    if (visible) {
        if (auto document_locked = document.lock()) {
            auto imgui_pos = ImGui::GetCursorScreenPos();
            auto imgui_size = ImGui::GetContentRegionAvail();
            render_position = {
                imgui_pos.x + imgui_size.x / 2.0f,
                imgui_pos.y + imgui_size.y / 2.0f
            };
            doc_service.set_active_document(document_locked);
        } else {
            std::cerr << "Invalid document" << std::endl;
            return true;
        }
    }
    ImGui::End();

    return !open;
}

const std::string& CanvasPanel::get_panel_name() const {
    return panel_name;
}

}
