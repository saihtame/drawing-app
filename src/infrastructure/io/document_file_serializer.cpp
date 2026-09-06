#include "document_file_serializer.hpp"


namespace Infrastructure::IO {

std::shared_ptr<Domain::Documents::Document> DocumentFileSerializer ::load_document(std::filesystem::path path) {
    return std::shared_ptr<Domain::Documents::Document>();
}

void DocumentFileSerializer::save_document(std::filesystem::path, const std::shared_ptr<Domain::Documents::Document>& document) {
}

}
