#include "application/documents/document_service.hpp"
#include "infrastructure/io/document_file_serializer.hpp"
#include "presentation/editor_runtime.hpp"


int main() {
    /* Initialize */
    Infrastructure::IO::DocumentFileSerializer doc_serializer;
    Application::Documents::DocumentService doc_service(doc_serializer);
    Presentation::EditorRuntime runtime(doc_service);

    /* Run */
    return runtime.run();
}
