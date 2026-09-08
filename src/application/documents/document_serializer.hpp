#pragma once
#include "domain/documents/document.hpp"
#include <filesystem>
#include <memory>


namespace Application::Documents {

class DocumentSerializer {
public:
    ~DocumentSerializer() = default;

public:
    virtual std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) const = 0;
    virtual void save_document(std::filesystem::path, const std::weak_ptr<Domain::Documents::Document>& document) const = 0;
};


}
