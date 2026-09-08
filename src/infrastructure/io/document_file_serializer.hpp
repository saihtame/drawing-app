#pragma once
#include "application/documents/document_serializer.hpp"
#include "application/documents/document_session.hpp"
#include <filesystem>
#include <memory>


namespace Infrastructure::IO {

class DocumentFileSerializer final : public Application::Documents::DocumentSerializer {
public:
    DocumentFileSerializer() = default;
    ~DocumentFileSerializer() = default;

public:
    virtual std::shared_ptr<Application::Documents::DocumentSession> load_document(std::filesystem::path path) override;
    virtual void save_document(std::filesystem::path, const std::shared_ptr<Application::Documents::DocumentSession>& document) override;
};

}
