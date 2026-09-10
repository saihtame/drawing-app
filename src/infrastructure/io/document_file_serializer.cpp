#include "document_file_serializer.hpp"
#include <glm/ext/vector_uint2.hpp>
#include "domain/documents/document.hpp"


namespace Infrastructure::IO {

DocumentFileSerializer::DocumentFileSerializer(const Domain::Documents::CanvasFactory& canvas_factory)
 : canvas_factory(canvas_factory) {
}

std::shared_ptr<Domain::Documents::Document> DocumentFileSerializer::load_document(std::filesystem::path path) const {
    auto doc = std::make_shared<Domain::Documents::Document>(canvas_factory, glm::uvec2());
    return doc;
}

void DocumentFileSerializer::save_document(std::filesystem::path, const std::weak_ptr<Domain::Documents::Document> &document) const {

}

}
