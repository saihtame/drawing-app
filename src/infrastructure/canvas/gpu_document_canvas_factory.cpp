#include "gpu_document_canvas_factory.hpp"
#include "application/documents/document_canvas.hpp"
#include "infrastructure/canvas/gpu_document_canvas.hpp"


namespace Infrastructure::Canvas {


std::shared_ptr<Application::Documents::DocumentCanvas> GpuDocumentCanvasFactory::create_canvas(const std::shared_ptr<Domain::Documents::Document>& document) {
    return std::shared_ptr<Application::Documents::DocumentCanvas>();
}

}
