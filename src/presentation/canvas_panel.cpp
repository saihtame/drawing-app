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

CanvasPanel::CanvasPanel(std::weak_ptr<Domain::Documents::Document> document)
 : document(document) {
    panel_no = get_canvas_number();
    panel_name = "Image " + std::to_string(panel_no);
}

CanvasPanel::~CanvasPanel() {
    return_canvas_number(panel_no);
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
