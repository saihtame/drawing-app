#include "pan_tool.hpp"


namespace Infrastructure::Tools {

void PanTool::start_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::start_stroke(position);
    std::cout << "PanTool stroke started" << std::endl;
}

void PanTool::continue_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::continue_stroke(position);
    std::cout << "PanTool stroke continued" << std::endl;
}

void PanTool::stop_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::stop_stroke(position);
    std::cout << "PanTool stroke stopped" << std::endl;
}

}
