#include "file_document_service.hpp"
#include "domain/documents/document.hpp"


namespace IO::Documents {

std::shared_ptr<Domain::Documents::Document> FileDocumentService::load_document(std::filesystem::path path) {
    return std::shared_ptr<Domain::Documents::Document>();
}

void FileDocumentService::save_document(std::filesystem::path, const std::shared_ptr<Domain::Documents::Document>& document) {
}

}
