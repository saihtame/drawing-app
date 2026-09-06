#pragma once
#include "application/documents/document_service.hpp"


namespace IO::Documents {

class FileDocumentService : public Application::Documents::DocumentService {
public:
    virtual std::shared_ptr<Domain::Documents::Document> load_document(std::filesystem::path path) override;
    virtual void save_document(std::filesystem::path, std::shared_ptr<Domain::Documents::Document> document) override;
};

}
