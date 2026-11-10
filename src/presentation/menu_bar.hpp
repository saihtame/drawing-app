#pragma once
#include "application/documents/document_service.hpp"
#include "canvas_panels/canvas_panel_service.hpp"
#include "popups/popup_manager.hpp"


namespace Presentation {

class MenuBar {
public:
    MenuBar(CanvasPanels::CanvasPanelService& canvas_panel_service,
            Application::Documents::DocumentService& doc_service,
            Popups::PopupManager& popup_manager);
    ~MenuBar() = default;

    void render();

private: // File menu
    inline void new_file();
    inline void load_file();
    inline void save_file();
    inline void save_file_as();
    inline void export_file();

private: // View menu
    inline void fit_to_screen();
    inline void set_zoom(float value);
    inline void center_canvas();

private: // Dependencies
    CanvasPanels::CanvasPanelService& canvas_panel_service;
    Application::Documents::DocumentService& doc_service;
    Popups::PopupManager& popup_manager;
};

}
