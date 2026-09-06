#pragma once
#include "domain/documents/document.hpp"
#include <filesystem>
#include <memory>


namespace Application::Documents {

class DocumentSerializer {
public:
    virtual std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) = 0;
    virtual void save_document(std::filesystem::path, const std::shared_ptr<Domain::Documents::Document>& document) = 0;
};

}
