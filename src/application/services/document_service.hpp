#pragma once
#include <memory>
#include "domain/document.hpp"


namespace Application::Services {

class DocumentService {
public:
    DocumentService() = default;
    ~DocumentService() = default;

public:
    std::shared_ptr<Domain::Document> get_active_document() const;
    void set_active_document(std::shared_ptr<Domain::Document> value);

private:
    std::shared_ptr<Domain::Document> active_document = nullptr;
};

}
