#pragma once
#include "application/documents/document_canvas.hpp"
#include "domain/documents/document.hpp"
#include <filesystem>
#include <optional>


namespace Application::Documents {

struct DocumentSession {
    DocumentSession(std::shared_ptr<Domain::Documents::Document> document,
                    std::shared_ptr<DocumentCanvas> canvas)
     : document(document), canvas(canvas) {}
    ~DocumentSession() = default;

    std::shared_ptr<Domain::Documents::Document> document = nullptr;
    std::shared_ptr<DocumentCanvas> canvas = nullptr;
    std::optional<std::filesystem::path> save_path;
};

}
