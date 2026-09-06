#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include "application/documents/document_serializer.hpp"
#include "domain/documents/document.hpp"


namespace Application::Documents {

class DocumentService {
public:
    DocumentService(DocumentSerializer& serializer);
    ~DocumentService() = default;

private: // Dependencies
    DocumentSerializer& serializer;

public:
    void add_document(std::shared_ptr<Domain::Documents::Document> document);
    void close_document();
    const std::list<std::shared_ptr<Domain::Documents::Document>>& get_loaded_documents() const;

public: // Save and load
    void load_document(std::filesystem::path path);
    void save_document(std::filesystem::path path);
    std::optional<std::filesystem::path> get_save_path(const std::shared_ptr<Domain::Documents::Document>& document) const;

public: // Active document
    inline bool has_active_document() const { return active_document != nullptr; }
    std::shared_ptr<Domain::Documents::Document> get_active_document() const;
    void set_active_document(std::shared_ptr<Domain::Documents::Document> value);
    void clear_active_document();

private: // Members
    std::shared_ptr<Domain::Documents::Document> active_document = nullptr;
    std::list<std::shared_ptr<Domain::Documents::Document>> loaded_documents;
    std::unordered_map<
        std::shared_ptr<Domain::Documents::Document>,
        std::filesystem::path
    > save_paths;
};

}
