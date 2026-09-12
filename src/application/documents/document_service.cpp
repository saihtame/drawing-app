#include "document_service.hpp"
#include "document_serializer.hpp"
#include <iostream>
#include <stdexcept>


namespace Application::Documents {

DocumentService::DocumentService(const DocumentSerializer& serializer,
                                 const Domain::Documents::CanvasFactory& canvas_factory)
 : serializer(serializer), canvas_factory(canvas_factory) {
}

void DocumentService::new_document(glm::uvec2 size) {
    auto document = std::make_shared<Domain::Documents::Document>(canvas_factory, size);
    // Create first layer
    auto canvas = document->get_canvas();
    if (auto canvas_locked = canvas.lock()) {
        auto layer = canvas_locked->add_layer();
        // Fill layer with white
        if (auto layer_locked = layer.lock()) {
            layer_locked->fill({1.0, 1.0, 1.0, 1.0});
        }
    } else {
        std::cerr << "Failed to initial canvas layer" << std::endl;
    }
    documents.insert({document->get_uuid(), document});
    notify_observers();
#ifdef _DEBUG
    std::cout << "Created new document" << std::endl;
#endif
}

const std::unordered_map<UUIDv4::UUID, std::shared_ptr<Domain::Documents::Document>>& DocumentService::get_documents() const {
    return documents;
}

std::shared_ptr<Domain::Documents::Document> DocumentService::get_document(const UUIDv4::UUID& uuid) const {
    auto it = documents.find(uuid);
    if (it == documents.end()) {
        return nullptr;
    }
    return it->second;
}

void DocumentService::close_document(const UUIDv4::UUID& uuid) {
    auto it = documents.find(uuid);
    if (it == documents.end()) {
        throw std::invalid_argument("Document not part of document service.");
    }

    documents.erase(it);
    notify_observers();
#ifdef _DEBUG
    std::cout << "Closed document" << std::endl;
#endif
}

void DocumentService::set_active_document(std::shared_ptr<Domain::Documents::Document> document) {
    if (active_document_uuid == document->get_uuid()) return;
    active_document_uuid = document->get_uuid();
    notify_observers();
}

[[nodiscard]] std::weak_ptr<Domain::Documents::Document> DocumentService::get_active_document() const {
    return get_document(active_document_uuid);
}

void DocumentService::load_document(std::filesystem::path path) {
    auto document = serializer.load_document(path);
    if (document == nullptr) {
        std::cerr << "Failed to load document at " << path << std::endl;
        return;
    }

    documents.insert({document->get_uuid(), document});
    notify_observers();
#ifdef _DEBUG
    std::cout << "Loaded document from " << path << std::endl;
#endif
}

void DocumentService::save_document(std::filesystem::path path, std::weak_ptr<Domain::Documents::Document> document) {
    serializer.save_document(path, document);

#ifdef _DEBUG
    std::cout << "Saved document to " << path << std::endl;
#endif
}

void DocumentService::register_observer(std::weak_ptr<DocumentServiceObserver> observer) {
    // Check value
    if (observer.expired()) {
        throw std::invalid_argument("Observer expired");
    }
    observers.push_back(observer);
}

void DocumentService::deregister_observer(std::weak_ptr<DocumentServiceObserver> observer) {
    std::erase_if(observers, [&](const std::weak_ptr<DocumentServiceObserver>& wp) {
        return !wp.owner_before(observer) && !observer.owner_before(wp);
    });
}

inline void DocumentService::notify_observers() {
    // In the case that a notify results in a deregister,
    // we need to avoid iterating a live vector. Hence the copy.
    const auto observers_copy = observers;
    for (auto& observer : observers_copy) {
        if (auto observer_locked = observer.lock())
            observer_locked->document_service_update();
    }
}

}
