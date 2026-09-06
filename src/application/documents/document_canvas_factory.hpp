#pragma once
#include "document_canvas.hpp"
#include "domain/documents/document.hpp"


namespace Application::Documents {

class DocumentCanvasFactory {
public:
    virtual std::shared_ptr<DocumentCanvas> create_canvas(const std::shared_ptr<Domain::Documents::Document>& document) = 0;
};

}
