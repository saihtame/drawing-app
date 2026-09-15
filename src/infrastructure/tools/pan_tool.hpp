#pragma once
#include "application/documents/document_service.hpp"
#include "domain/tools/stroke_tool.hpp"


namespace Infrastructure::Tools {

class PanTool final : public Domain::Tools::StrokeTool {
public: // Ctor
    PanTool(Application::Documents::DocumentService& doc_service)
        : doc_service(doc_service) {}
    ~PanTool() = default;

public: // StrokeTool implementation
    void start_stroke(glm::vec2 position) override;
    void continue_stroke(glm::vec2 position) override;
    void stop_stroke(glm::vec2 position) override;

public: // Tool implementation
    std::string get_tool_name() const override { return "Pan"; }
    Domain::Tools::ToolType get_tool_type() const override { return Domain::Tools::TOOL_TYPE_PAN; }

private: // Dependencies
    Application::Documents::DocumentService& doc_service;
};

}
