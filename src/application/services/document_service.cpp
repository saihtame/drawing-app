#include "document_service.hpp"

namespace Application::Services {

std::shared_ptr<Domain::Document> DocumentService::get_active_document() const {
    return active_document;
}

void DocumentService::set_active_document(std::shared_ptr<Domain::Document> value) {
    active_document = std::move(value);
}

}