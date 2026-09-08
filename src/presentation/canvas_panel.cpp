#include "canvas_panel.hpp"
#include "imgui.h"


namespace Presentation {


// Each panel just uses this no for its ImGui window name and increments the value.
// This ensures no two panels have the same name.
static int canvas_panels_instantiated = 0;

CanvasPanel::CanvasPanel(std::weak_ptr<Domain::Documents::Document> document)
 : document(document) {
    panel_name = std::to_string(canvas_panels_instantiated++);
}

bool CanvasPanel::render() {
    ImGui::Begin(panel_name.c_str(), &open, flags);
    ImGui::Text("I start docked on the center.");
    ImGui::End();

    return !open;
}

const std::string& CanvasPanel::get_panel_name() const {
    return panel_name;
}

}
