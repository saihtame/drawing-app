#pragma once
#include "application/services/document_service.hpp"


namespace Presentation {

class EditorRuntime {
public:
    EditorRuntime(std::shared_ptr<Application::Services::DocumentService> doc_service);
    ~EditorRuntime() = default;

private: // Dependencies
    std::shared_ptr<Application::Services::DocumentService> doc_service;

public:
    int run();

private:
    inline void initialize_window();
};

}
