#include "application/documents/document_service.hpp"
#include "infrastructure/canvas/gpu_document_canvas_factory.hpp"
#include "infrastructure/io/document_file_serializer.hpp"
#include "presentation/editor_runtime.hpp"


int main() {
    /* Initialize */
    Infrastructure::Canvas::GpuDocumentCanvasFactory canvas_factory;
    Infrastructure::IO::DocumentFileSerializer doc_serializer;
    Application::Documents::DocumentService doc_service(
        doc_serializer, canvas_factory);
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
