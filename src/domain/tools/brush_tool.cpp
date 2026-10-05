#include "brush_tool.hpp"
#include <algorithm>


namespace Domain::Tools {

void BrushTool::set_size(float value) {
    settings.size = std::clamp(value, MIN_SIZE, MAX_SIZE);
}

void BrushTool::set_hardness(float value) {
    settings.hardness = std::clamp(value, MIN_HARDNESS, MAX_HARDNESS);
}

void BrushTool::set_opacity(float value) {
    settings.opacity = std::clamp(value, MIN_OPACITY, MAX_OPACITY);
}

void BrushTool::set_color(glm::vec4 value) {
    settings.color = {
        std::clamp(value.x, 0.0f, 1.0f),
        std::clamp(value.y, 0.0f, 1.0f),
        std::clamp(value.z, 0.0f, 1.0f),
        std::clamp(value.w, 0.0f, 1.0f),
    };
}

}
