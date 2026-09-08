#include "canvas_panel.hpp"
#include "imgui.h"
#include <iostream>


namespace Presentation {


// Each panel just uses this no for its ImGui window name and increments the value.
// This ensures no two panels have the same name.
static int canvas_panels_instantiated = 0;

CanvasPanel::CanvasPanel(std::weak_ptr<Domain::Documents::Document> document)
 : document(document) {
    panel_name = std::to_string(canvas_panels_instantiated++);
}

bool CanvasPanel::render() {
    if (document.expired())
        return false;

    bool visible = ImGui::Begin(panel_name.c_str(), &open, flags);
    if (visible) {
        if (auto doc = document.lock()) {
            auto canvas = doc->get_canvas();
            auto imgui_size = ImGui::GetContentRegionAvail();
            canvas.render({imgui_size.x, imgui_size.y});
        } else {
            std::cerr << "CanvasPanel has invalid document" << std::endl;
            open = false;
        }
    }
    ImGui::End();

    return !open;
}

const std::string& CanvasPanel::get_panel_name() const {
    return panel_name;
}

}
