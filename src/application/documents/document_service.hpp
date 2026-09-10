#pragma once
#include <filesystem>
#include <memory>
#include <unordered_set>
#include "domain/documents/canvas_factory.hpp"
#include "domain/documents/document.hpp"
#include "document_serializer.hpp"


namespace Application::Documents {

class DocumentServiceObserver {
public:
    virtual void document_service_update() = 0;
};

class DocumentService final {
public:
    DocumentService(const DocumentSerializer& serializer,
                    const Domain::Documents::CanvasFactory& canvas_factory);
    ~DocumentService() = default;

private: // Dependencies
    const DocumentSerializer& serializer;
    const Domain::Documents::CanvasFactory& canvas_factory;

public: // Documents
    void new_document(glm::uvec2 size);
    const std::unordered_set<std::shared_ptr<Domain::Documents::Document>>& get_documents() const;
    void close_document(const std::shared_ptr<Domain::Documents::Document>& document);

public: // Active document
    void set_active_document(std::shared_ptr<Domain::Documents::Document> document);
    [[nodiscard]] std::weak_ptr<Domain::Documents::Document> get_active_document() const;

public: // Serialization
    void load_document(std::filesystem::path path);
    void save_document(std::filesystem::path path, std::weak_ptr<Domain::Documents::Document> document);

public: // Observers
    void register_observer(std::weak_ptr<DocumentServiceObserver> observer);
    void deregister_observer(std::weak_ptr<DocumentServiceObserver> observer);
private:
    inline void notify_observers();

private: // Data
    std::unordered_set<std::shared_ptr<Domain::Documents::Document>> documents;
    std::vector<std::weak_ptr<DocumentServiceObserver>> observers;
    std::shared_ptr<Domain::Documents::Document> active_document;
};

}
