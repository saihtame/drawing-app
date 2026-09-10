#pragma once
#include "application/documents/document_service.hpp"


namespace Presentation {

class LayersPanel final {
public: // Ctors
    LayersPanel(Application::Documents::DocumentService& doc_service) : doc_service(doc_service) {}
    ~LayersPanel() = default;

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

public:
    void render();
};

}
