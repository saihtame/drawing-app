#include "pan_tool.hpp"
#include <chrono>


namespace Infrastructure::Tools {

void PanTool::start_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::start_stroke(position);
    // Save start pan position
    last_pan_position = position;
    std::cout << "PanTool stroke started" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}

void PanTool::continue_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::continue_stroke(position);
    // Move the canvas offset
    auto doc = doc_service.get_active_document().lock();
    if (doc != nullptr) {
        auto canvas = doc->get_canvas().lock();
        if (canvas != nullptr) {
            glm::vec2 offset = position - last_pan_position;
            canvas->set_rendering_offset(canvas->get_rendering_offset() + offset);
        }
    }
    // Update last pan position
    last_pan_position = position;
    std::cout << "PanTool stroke continued" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}

void PanTool::stop_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::stop_stroke(position);
    std::cout << "PanTool stroke stopped" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}

}
