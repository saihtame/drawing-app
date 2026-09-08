#pragma once
#include <filesystem>
#include <memory>
#include "application/documents/document_canvas_factory.hpp"
#include "domain/documents/document.hpp"
#include "document_serializer.hpp"
#include "document_session.hpp"


namespace Application::Documents {

class DocumentServiceObserver {
public:
    virtual void document_service_update() = 0;
};

class DocumentService final {
public:
    DocumentService(DocumentSerializer& serializer,
                    DocumentCanvasFactory& canvas_factory);
    ~DocumentService() = default;

private: // Dependencies
    DocumentSerializer& serializer;
    DocumentCanvasFactory& canvas_factory;

public:
    void add_document(std::shared_ptr<Domain::Documents::Document> document);
    void close_document();
    const std::list<std::shared_ptr<DocumentSession>>& get_loaded_documents() const;

public: // Save and load
    void load_document(std::filesystem::path path);
    void save_document(std::filesystem::path path);

public: // Sessions
    void add_session(std::shared_ptr<DocumentSession> session);
    inline bool has_active_session() const { return active_session != nullptr; }
    std::shared_ptr<DocumentSession> get_active_session() const;
    void set_active_session(std::shared_ptr<DocumentSession> value);
    void clear_active_session();

public: // Observers
    void register_observer(std::weak_ptr<DocumentServiceObserver> observer);
    void deregister_observer(std::weak_ptr<DocumentServiceObserver> observer);
private:
    inline void notify_observers();

private: // Members
    std::shared_ptr<DocumentSession> active_session = nullptr;
    std::list<std::shared_ptr<DocumentSession>> sessions;
    std::vector<std::weak_ptr<DocumentServiceObserver>> observers;
};

}
