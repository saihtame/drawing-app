#include "document_service.hpp"
#include <algorithm>
#include <stdexcept>


namespace Application::Documents {

void DocumentService::add_document(std::shared_ptr<Domain::Documents::Document> document) {
    if (document == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    loaded_documents.push_back(document);
}

void DocumentService::close_document(std::shared_ptr<Domain::Documents::Document> document) {
    if (document == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    auto it = std::find_if(loaded_documents.begin(), loaded_documents.end(),
        [&](const std::shared_ptr<Domain::Documents::Document>& d) {
            return d && *d == *document; // compares Document::operator==
        });

    if (it == loaded_documents.end()) {
        throw std::invalid_argument("Document is not loaded by the document service.");
    }
}

std::shared_ptr<Domain::Documents::Document> DocumentService::get_active_document() const {
    return active_document;
}

void DocumentService::set_active_document(std::shared_ptr<Domain::Documents::Document> value) {
    if (value == nullptr) {
        throw std::invalid_argument("Value is null");
    }

    active_document = std::move(value);
}

std::list<std::shared_ptr<Domain::Documents::Document>> DocumentService::get_loaded_documents() const {
    return loaded_documents;
}

}
