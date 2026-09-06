#pragma once
#include <filesystem>
#include <memory>
#include "domain/documents/document.hpp"


namespace Application::Documents {

class DocumentService {
public:
    DocumentService() = default;
    ~DocumentService() = default;

public:
    void add_document(std::shared_ptr<Domain::Documents::Document> document);
    void close_document(std::shared_ptr<Domain::Documents::Document> document);
    std::shared_ptr<Domain::Documents::Document> get_active_document() const;
    void set_active_document(std::shared_ptr<Domain::Documents::Document> value);
    std::list<std::shared_ptr<Domain::Documents::Document>> get_loaded_documents() const;

public: // Virtual methods
    virtual std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) = 0;
    virtual void save_document(std::filesystem::path, std::shared_ptr<Domain::Documents::Document> document) = 0;

private:
    std::shared_ptr<Domain::Documents::Document> active_document = nullptr;
    std::list<std::shared_ptr<Domain::Documents::Document>> loaded_documents;
};

}
