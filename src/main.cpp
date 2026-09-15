#include "application/documents/document_service.hpp"
#include "application/tools/toolbar.hpp"
#include "infrastructure/canvas/gpu_canvas_factory.hpp"
#include "infrastructure/io/document_file_serializer.hpp"
#include "infrastructure/tools/brush_tool.hpp"
#include "infrastructure/tools/pan_tool.hpp"
#include "presentation/editor_runtime.hpp"


int main() {
    /* Initialize and dependency inject */
    // Document and canvas
    Infrastructure::Canvas::GpuCanvasFactory canvas_factory;
    Infrastructure::IO::DocumentFileSerializer doc_serializer(canvas_factory);
    Application::Documents::DocumentService doc_service(doc_serializer, canvas_factory);
    // Tools
    std::vector<std::shared_ptr<Domain::Tools::Tool>> tools = {
        // Pan
        std::make_shared<Infrastructure::Tools::PanTool>(doc_service),
        // Brush
        std::make_shared<Infrastructure::Tools::BrushTool>(doc_service),
    };
    Application::Tools::Toolbar toolbar(tools);
    // Runtime
    Presentation::EditorRuntime runtime(doc_service, toolbar);

    /* Run */
    return runtime.run();
}
