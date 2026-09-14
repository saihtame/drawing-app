#include "brush_tool.hpp"
#include "domain/tools/stroke_tool.hpp"


namespace Infrastructure::Tools {

void BrushTool::start_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::start_stroke(position);
    std::cout << "BrushTool stroke started" << std::endl;
}

void BrushTool::continue_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::continue_stroke(position);
    std::cout << "BrushTool stroke continued" << std::endl;
}

void BrushTool::stop_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::stop_stroke(position);
    std::cout << "BrushTool stroke stopped" << std::endl;
}

}
