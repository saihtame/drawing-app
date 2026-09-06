#include "document_service.hpp"
#include <algorithm>
#include <optional>
#include <stdexcept>


namespace Application::Documents {

void DocumentService::add_document(std::shared_ptr<Domain::Documents::Document> document) {
    // Check for null argument
    if (document == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    loaded_documents.push_back(std::move(document));
}

void DocumentService::close_document(const std::shared_ptr<Domain::Documents::Document>& document) {
    // Check for null argument
    if (document == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    // Find match
    auto it = std::find(loaded_documents.begin(), loaded_documents.end(), document);
    if (it == loaded_documents.end()) {
        throw std::invalid_argument("Document is not loaded by the document service.");
    }

    // Erase from loaded documents
    loaded_documents.erase(it);

    // Erase save path, if it exists
    auto save_it = save_paths.find(document);
    if (save_it != save_paths.end()) {
        save_paths.erase(save_it);
    }
}

std::optional<std::filesystem::path> DocumentService::get_save_path(const std::shared_ptr<Domain::Documents::Document>& document) {
    auto it = save_paths.find(document);
    if (it == save_paths.end()) {
        return std::optional<std::filesystem::path>();
    }
    return it->second;
}

std::shared_ptr<Domain::Documents::Document> DocumentService::get_active_document() const {
    return active_document;
}

void DocumentService::set_active_document(std::shared_ptr<Domain::Documents::Document> value) {
    // Check for null argument
    if (value == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    active_document = std::move(value);
}

std::list<std::shared_ptr<Domain::Documents::Document>> DocumentService::get_loaded_documents() const {
    return loaded_documents;
}

}
