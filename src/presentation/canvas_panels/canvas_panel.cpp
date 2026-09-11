#include "canvas_panel.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <iostream>
#include <limits>
#include <unordered_set>


namespace Presentation::CanvasPanels {


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
    // Make ImGui forget our window name. So that it can be cleanly reused.
    ImGui::ClearWindowSettings(panel_name.c_str());
}

bool CanvasPanel::render() {
    if (document.expired())
        return false;

    bool visible = ImGui::Begin(panel_name.c_str(), &open, flags);

    // Check for first render
    if (ImGui::IsWindowAppearing()) {
        if (auto document_locked = document.lock()) {
            // Fit canvas to screen
            if (auto canvas = document_locked->get_canvas().lock()) {
                auto screen_size = ImGui::GetContentRegionAvail();
                canvas->fit_screen({screen_size.x, screen_size.y});
            }
        }
    }

    // If panel is visible, render our canvas
    if (visible) {
        if (auto document_locked = document.lock()) {
            // Ensure document is set as active document
            doc_service.set_active_document(document_locked);

            // Render document canvas
            auto imgui_pos = ImGui::GetCursorScreenPos();
            auto imgui_size = ImGui::GetContentRegionAvail();
            glm::vec2 render_position = {
                imgui_pos.x + imgui_size.x / 2.0f,
                imgui_pos.y + imgui_size.y / 2.0f
            };
            auto canvas = document_locked->get_canvas();
            if (auto canvas_locked = canvas.lock()) {
                canvas_locked->render(render_position);
            } else {
                std::cerr << "Document has invalid canvas" << std::endl;
                return true;
            }
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
