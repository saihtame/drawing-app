#pragma once
#include "application/documents/document_serializer.hpp"
#include <filesystem>
#include <memory>


namespace Infrastructure::IO {

class DocumentFileSerializer final : public Application::Documents::DocumentSerializer {
public:
    DocumentFileSerializer(const Domain::Documents::CanvasFactory& canvas_layer_factory);
    ~DocumentFileSerializer() = default;

private: // Dependencies
    const Domain::Documents::CanvasFactory& canvas_factory;

public:
    std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) const override;
    void save_document(std::filesystem::path, const std::weak_ptr<Domain::Documents::Document> &document) const override;
};

}
