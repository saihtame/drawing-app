#include "document_service.hpp"
#include "application/documents/document_serializer.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <stdexcept>


namespace Application::Documents {

DocumentService::DocumentService(DocumentSerializer& serializer)
 : serializer(serializer) {
}

void DocumentService::add_document(std::shared_ptr<Domain::Documents::Document> document) {
    // Check for null argument
    if (document == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    loaded_documents.push_back(document);
    set_active_document(document);
}

void DocumentService::close_document() {
    auto document = get_active_document();
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

    clear_active_document();
}

void DocumentService::load_document(std::filesystem::path path) {
    auto doc = serializer.load_document(path);
    if (doc == nullptr) {
        std::cerr << "Failed to load document at " << path << std::endl;
        return;
    }

    add_document(doc);
#ifdef _DEBUG
    std::cout << "Loaded document from " << path << std::endl;
#endif
}

void DocumentService::save_document(std::filesystem::path path) {
    if (active_document == nullptr)
        throw std::runtime_error("No active document to save");

    serializer.save_document(path, active_document);
    save_paths[active_document] = path;

#ifdef _DEBUG
    std::cout << "Saved document to " << path << std::endl;
#endif
}

std::optional<std::filesystem::path> DocumentService::get_save_path(const std::shared_ptr<Domain::Documents::Document>& document) const {
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

    // Ensure document has been added to loaded documents
    auto it = std::find(loaded_documents.begin(), loaded_documents.end(), value);
    if (it == loaded_documents.end()) {
        throw std::invalid_argument("Cannot set active document, before adding it.");
    }

    active_document = std::move(value);
}

const std::list<std::shared_ptr<Domain::Documents::Document>>&
    DocumentService::get_loaded_documents() const {
    return loaded_documents;
}

void DocumentService::clear_active_document() {
    active_document = nullptr;
}

}
