#include "presentation/editor_runtime.hpp"
#include "io/documents/file_document_service.hpp"


int main() {
    /* Initialize */
    // Application
    auto doc_service = std::make_shared<IO::Documents::FileDocumentService>();
    // Presentation
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
