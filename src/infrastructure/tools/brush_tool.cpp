#include "brush_tool.hpp"
#include "domain/tools/stroke_tool.hpp"
#include "infrastructure/compute/brush_painter.hpp"


namespace Infrastructure::Tools {

BrushTool::BrushTool(Application::Documents::DocumentService& doc_service) : doc_service(doc_service) {
}

BrushTool::~BrushTool() {}

void BrushTool::start_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::start_stroke(position);
    // Initialize brush painter shader program only when we try to use it
    // This is to ensure the shader is only compiled once OpenGL has been initialized
    if (brush_painter == nullptr)
            brush_painter = std::make_unique<Compute::BrushPainter>();

    std::cout << "BrushTool stroke started" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}

void BrushTool::continue_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::continue_stroke(position);
    std::cout << "BrushTool stroke continued" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}

void BrushTool::stop_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::stop_stroke(position);
    std::cout << "BrushTool stroke stopped" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}

}
