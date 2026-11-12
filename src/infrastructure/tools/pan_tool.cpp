#include "pan_tool.hpp"


namespace Infrastructure::Tools {

void PanTool::start_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::start_stroke(position);
    // Save start pan position
    last_pan_position = position;
}

void PanTool::continue_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::continue_stroke(position);
    // Move the canvas offset
    auto doc = doc_service.get_active_document().lock();
    if (doc != nullptr) {
        auto& canvas = doc->get_canvas();
        glm::vec2 offset = position - last_pan_position;
        canvas.set_rendering_offset(canvas.get_rendering_offset() + offset);
    }
    // Update last pan position
    last_pan_position = position;
}

void PanTool::stop_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::stop_stroke(position);
}

}
