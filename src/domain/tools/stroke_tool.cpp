#include "stroke_tool.hpp"
#include <iostream>


namespace Domain::Tools {

void StrokeTool::start_stroke(glm::vec2 position) {
    if (in_stroke) {
        std::cerr << "StokeTool started before ending last one" << std::endl;
        return;
    }
    in_stroke = true;
}

void StrokeTool::continue_stroke(glm::vec2 position) {
    if (!in_stroke) {
        std::cerr << "StokeTool cannot continue stroke, before starting it." << std::endl;
        return;
    }
}

void StrokeTool::stop_stroke(glm::vec2 position) {
    if (!in_stroke) {
        std::cerr << "StokeTool cannot stop stroke, before starting it." << std::endl;
        return;
    }
    in_stroke = false;
}

}
