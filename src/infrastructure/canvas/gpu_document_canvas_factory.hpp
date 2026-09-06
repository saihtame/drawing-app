#pragma once
#include "application/documents/document_canvas_factory.hpp"


namespace Infrastructure::Canvas {

class GpuDocumentCanvasFactory final : public Application::Documents::DocumentCanvasFactory {
    virtual std::shared_ptr<Application::Documents::DocumentCanvas> create_canvas(const std::shared_ptr<Domain::Documents::Document>& document) override;
};

}
