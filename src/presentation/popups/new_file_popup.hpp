#pragma once
#include "application/documents/document_service.hpp"
#include "popup.hpp"
#include <glm/ext/vector_uint2.hpp>


namespace Presentation::Popups {

class NewFilePopup : public Popup {
public:
    explicit NewFilePopup(Application::Documents::DocumentService& doc_service);
    ~NewFilePopup() = default;

public: // Overrides
    bool render() override;
    const char* get_name() const override { return "New file"; }

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

private: // Members
    glm::uvec2 size = glm::uvec2(640, 480);
};

}
