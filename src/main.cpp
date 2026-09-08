#include "application/documents/document_service.hpp"
#include "infrastructure/canvas/gpu_canvas_layer_factory.hpp"
#include "infrastructure/io/document_file_serializer.hpp"
#include "presentation/editor_runtime.hpp"


int main() {
    /* Initialize and dependency inject */
    Infrastructure::Canvas::GpuCanvasLayerFactory canvas_factory;
    Infrastructure::IO::DocumentFileSerializer doc_serializer(canvas_factory);
    Application::Documents::DocumentService doc_service(
        doc_serializer, canvas_factory);
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
