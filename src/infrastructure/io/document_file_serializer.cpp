#include "document_file_serializer.hpp"
#include "application/documents/document_session.hpp"


namespace Infrastructure::IO {

std::shared_ptr<Application::Documents::DocumentSession> DocumentFileSerializer ::load_document(std::filesystem::path path) {
    return std::shared_ptr<Application::Documents::DocumentSession>();
}

void DocumentFileSerializer::save_document(std::filesystem::path, const std::shared_ptr<Application::Documents::DocumentSession>& document) {
}

}
