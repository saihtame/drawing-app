#include "application/documents/document_service.hpp"
#include "io/documents/document_file_serializer.hpp"
#include "presentation/editor_runtime.hpp"


int main() {
    /* Initialize */
    IO::Documents::DocumentFileSerializer doc_serializer;
    Application::Documents::DocumentService doc_service(doc_serializer);
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
