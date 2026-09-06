#pragma once
#include "application/documents/document_service.hpp"


namespace Presentation {

class MenuBar {
public:
    MenuBar(Application::Documents::DocumentService& doc_service);
    ~MenuBar() = default;

    void render();

private: // Dependencies
    Application::Documents::DocumentService& doc_service;
};

}
