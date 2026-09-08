#pragma once
#include <filesystem>
#include <memory>
#include <unordered_set>
#include "domain/documents/canvas_layer_factory.hpp"
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
                    const Domain::Documents::CanvasLayerFactory& canvas_layer_factory);
    ~DocumentService() = default;

private: // Dependencies
    const DocumentSerializer& serializer;
    const Domain::Documents::CanvasLayerFactory& canvas_layer_factory;

public: // Documents
    void new_document(glm::uvec2 size);
    const std::unordered_set<std::shared_ptr<Domain::Documents::Document>>& get_documents() const;
    void close_document(const std::shared_ptr<Domain::Documents::Document>& document);

public: // Serialization
    void load_document(std::filesystem::path path);
    void save_document(std::filesystem::path path, std::weak_ptr<Domain::Documents::Document> document);

public: // Observers
    void register_observer(std::weak_ptr<DocumentServiceObserver> observer);
    void deregister_observer(std::weak_ptr<DocumentServiceObserver> observer);
private:
    inline void notify_observers();

private: // Members
    std::unordered_set<std::shared_ptr<Domain::Documents::Document>> documents;
    std::vector<std::weak_ptr<DocumentServiceObserver>> observers;
};

}
