#include "document_service.hpp"


namespace Application::Documents {

std::shared_ptr<Domain::Documents::Document> DocumentService::get_active_document() const {
    return active_document;
}

void DocumentService::set_active_document(std::shared_ptr<Domain::Documents::Document> value) {
    active_document = std::move(value);
}

std::list<std::shared_ptr<Domain::Documents::Document>> DocumentService::get_loaded_documents() const {
    return loaded_documents;
}

}
