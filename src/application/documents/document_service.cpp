#include "document_service.hpp"
#include "application/documents/document_serializer.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>


namespace Application::Documents {

DocumentService::DocumentService(DocumentSerializer& serializer,
                                 DocumentCanvasFactory& canvas_factory)
 : serializer(serializer), canvas_factory(canvas_factory) {
}

void DocumentService::add_document(std::shared_ptr<Domain::Documents::Document> document) {
    // Check for null argument
    if (document == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    // Create session object
    auto canvas = canvas_factory.create_canvas(document);
    auto session = std::make_shared<DocumentSession>(document, canvas);
    sessions.push_back(session);

    // Set new document to be active
    set_active_session(session);
}

void DocumentService::close_document() {
    auto session = get_active_session();
    // Check for null argument
    if (session == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    // Erase from loaded documents
    auto it = std::find(sessions.begin(), sessions.end(), session);
    if (it == sessions.end()) {
        throw std::invalid_argument("Document is not loaded by the document service.");
    }
    sessions.erase(it);

    clear_active_session();
}

void DocumentService::add_session(std::shared_ptr<DocumentSession> session) {
    // Ensure not already loaded
    auto it = std::find(sessions.begin(), sessions.end(), session);
    if (it != sessions.end()) {
        throw std::invalid_argument("Session already added");
    }

    sessions.push_back(*it);
    set_active_session(session);
}

void DocumentService::load_document(std::filesystem::path path) {
    auto session = serializer.load_document(path);
    if (session == nullptr) {
        std::cerr << "Failed to load document at " << path << std::endl;
        return;
    }

    add_session(session);
#ifdef _DEBUG
    std::cout << "Loaded document from " << path << std::endl;
#endif
}

void DocumentService::save_document(std::filesystem::path path) {
    if (active_session == nullptr)
        throw std::runtime_error("No active document to save");

    serializer.save_document(path, active_session);

#ifdef _DEBUG
    std::cout << "Saved document to " << path << std::endl;
#endif
}

std::shared_ptr<DocumentSession> DocumentService::get_active_session() const {
    return active_session;
}

void DocumentService::set_active_session(std::shared_ptr<DocumentSession> value) {
    // Check for null argument
    if (value == nullptr) {
        throw std::invalid_argument("Document is null");
    }

    // Ensure document has been added to loaded documents
    auto it = std::find(sessions.begin(), sessions.end(), value);
    if (it == sessions.end()) {
        throw std::invalid_argument("Cannot set active document, before adding it.");
    }

    active_session = std::move(value);
}

const std::list<std::shared_ptr<DocumentSession>>&
    DocumentService::get_loaded_documents() const {
    return sessions;
}

void DocumentService::clear_active_session() {
    active_session = nullptr;
}

}
