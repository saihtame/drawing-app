#pragma once
#include "application/documents/document_session.hpp"
#include <filesystem>
#include <memory>


namespace Application::Documents {

class DocumentSerializer {
public:
    ~DocumentSerializer() = default;

public:
    virtual std::shared_ptr<DocumentSession> load_document(std::filesystem::path path) = 0;
    virtual void save_document(std::filesystem::path, const std::shared_ptr<DocumentSession>& document) = 0;
};


}
