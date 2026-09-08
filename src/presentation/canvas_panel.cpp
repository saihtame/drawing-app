#include "canvas_panel.hpp"
#include "imgui.h"


namespace Presentation {


// Each panel just uses this no for its ImGui window name and increments the value.
// This ensures no two panels have the same name.
static int CanvasPanelsSpawned = 0;

CanvasPanel::CanvasPanel(std::weak_ptr<Application::Documents::DocumentCanvas> canvas)
 : canvas(canvas) {
    panel_name = std::to_string(CanvasPanelsSpawned++);
}

void CanvasPanel::render() {
    ImGui::Begin(panel_name.c_str(), &open, flags);
    ImGui::Text("I start docked on the center.");
    ImGui::End();
}

const std::string& CanvasPanel::get_panel_name() const {
    return panel_name;
}

}
