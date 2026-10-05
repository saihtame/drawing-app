#pragma once
#include "application/documents/document_service.hpp"
#include "domain/tools/brush_tool.hpp"
#include <memory>


// Forward declarations. Kept out of line so that <glad/gl.h>, which
// infrastructure/rendering/shader_program.hpp pulls in, never reaches the
// translation units that include this header (main.cpp included).
namespace Infrastructure::Compute {
class BrushPainter;
}
namespace Infrastructure::Canvas {
class GpuCanvasLayer;
}

namespace Infrastructure::Tools {

class BrushTool final : public Domain::Tools::BrushTool {
public: // Ctor
    BrushTool(Application::Documents::DocumentService& doc_service);
    ~BrushTool();

public: // StrokeTool implementation
    void start_stroke(glm::vec2 position) override;
    void continue_stroke(glm::vec2 position) override;
    void stop_stroke(glm::vec2 position) override;

public: // Tool implementation
    std::string get_tool_name() const override { return "Brush"; }
    Domain::Tools::ToolType get_tool_type() const override { return Domain::Tools::TOOL_TYPE_BRUSH; }

private: // Helpers
    std::weak_ptr<Canvas::GpuCanvasLayer> resolve_target_layer();
    void paint_segment(glm::vec2 from, glm::vec2 to);

private: // Data
    std::unique_ptr<Compute::BrushPainter> brush_painter;
    // Resolved once per stroke: the selected layer cannot change mid stroke
    std::weak_ptr<Canvas::GpuCanvasLayer> target_layer;
    glm::vec2 last_position = {0.0f, 0.0f};

private: // Dependencies
    Application::Documents::DocumentService& doc_service;
};

}
