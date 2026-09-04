#include "application/services/document_service.hpp"
#include "presentation/editor_runtime.hpp"


int main() {
    /* Initialize */
    // Application
    auto doc_service = std::make_shared<Application::Services::DocumentService>();
    // Presentation
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
