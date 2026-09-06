#pragma once
#include "application/documents/document_service.hpp"
#include "popups/popup_manager.hpp"


namespace Presentation {

class MenuBar {
public:
    MenuBar(Application::Documents::DocumentService& doc_service,
            Popups::PopupManager& popup_manager);
    ~MenuBar() = default;

    void render();

private: // Helpers
    inline void new_file();
    inline void load_file();
    inline void save_file();
    inline void save_file_as();

private: // Dependencies
    Application::Documents::DocumentService& doc_service;
    Popups::PopupManager& popup_manager;
};

}
