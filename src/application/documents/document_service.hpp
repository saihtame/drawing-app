#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include "domain/documents/document.hpp"


namespace Application::Documents {

class DocumentService {
public:
    DocumentService() = default;
    ~DocumentService() = default;

public:
    void add_document(std::shared_ptr<Domain::Documents::Document> document);
    void close_document(const std::shared_ptr<Domain::Documents::Document>& document);
    std::shared_ptr<Domain::Documents::Document> get_active_document() const;
    void set_active_document(std::shared_ptr<Domain::Documents::Document> value);
    std::list<std::shared_ptr<Domain::Documents::Document>> get_loaded_documents() const;
    inline bool has_active_document() const { return active_document != nullptr; }
    std::optional<std::filesystem::path> get_save_path(const std::shared_ptr<Domain::Documents::Document>& document);

public: // Virtual methods
    virtual std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) = 0;
    virtual void save_document(std::filesystem::path, const std::shared_ptr<Domain::Documents::Document>& document) = 0;

private: // Members
    std::shared_ptr<Domain::Documents::Document> active_document = nullptr;
    std::list<std::shared_ptr<Domain::Documents::Document>> loaded_documents;
    std::unordered_map<
        std::shared_ptr<Domain::Documents::Document>,
        std::filesystem::path
    > save_paths;
};

}
