#pragma once
#include "application/documents/document_service.hpp"
#include "domain/tools/stroke_tool.hpp"


// Forward decleration
namespace Infrastructure::Compute {
class BrushPainter;
}

namespace Infrastructure::Tools {

class BrushTool final : public Domain::Tools::StrokeTool {
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

private: // Data
    std::unique_ptr<Compute::BrushPainter> brush_painter;

private: // Dependencies
    Application::Documents::DocumentService& doc_service;
};

}
