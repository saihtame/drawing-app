#include "presentation/editor_runtime.hpp"
#include "io/documents/file_document_service.hpp"


int main() {
    /* Initialize */
    // Application
    IO::Documents::FileDocumentService doc_service;
    // Presentation
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
