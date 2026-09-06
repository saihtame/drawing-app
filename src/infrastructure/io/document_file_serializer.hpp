#pragma once
#include "application/documents/document_serializer.hpp"
#include <filesystem>
#include <memory>


namespace Infrastructure::IO {

class DocumentFileSerializer : public Application::Documents::DocumentSerializer {
public:
    DocumentFileSerializer() = default;
    ~DocumentFileSerializer() = default;

public:
    virtual std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) override;
    virtual void save_document(std::filesystem::path, const std::shared_ptr<Domain::Documents::Document>& document) override;
};

}
