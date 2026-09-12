#include "menu_bar.hpp"
#include "imgui.h"
#include "presentation/popups/new_file_popup.hpp"


namespace  Presentation {

MenuBar::MenuBar(CanvasPanels::CanvasPanelService& canvas_panel_service,
                 Application::Documents::DocumentService& doc_service,
                 Popups::PopupManager& popup_manager)
 : canvas_panel_service(canvas_panel_service), doc_service(doc_service), popup_manager(popup_manager) {
}

void MenuBar::render() {
    if(ImGui::BeginMainMenuBar()) {
        /* File menu */
        if (ImGui::BeginMenu("File", true)) {
            // New File
            if (ImGui::MenuItem("New file")) {
                new_file();
            }
            // Load File
            if (ImGui::MenuItem("Load file")) {
                load_file();
            }
            bool can_save = false;
            // Save File
            if (ImGui::MenuItem("Save file", "", false, can_save)) {
                save_file();
            }
            // Save File As
            if (ImGui::MenuItem("Save file as", "", false, can_save)) {
                save_file_as();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View", true)) {
            // New File
            if (ImGui::MenuItem("Fit to screen")) {
                fit_to_screen();
            }
            ImGui::EndMenu();
        }
        /* View menu */
        ImGui::EndMainMenuBar();
    }
}

inline void MenuBar::new_file() {
    auto popup = std::make_shared<Popups::NewFilePopup>(doc_service);
    popup_manager.push_popup(popup);
}

inline void MenuBar::load_file() {
}

inline void MenuBar::save_file() {
}

inline void MenuBar::save_file_as() {
}

inline void MenuBar::fit_to_screen() {
    auto active_document = doc_service.get_active_document();
    if (auto document_locked = active_document.lock()) {
        if (auto canvas = document_locked->get_canvas().lock()) {
            auto panel = canvas_panel_service.get_canvas_panel(document_locked->get_uuid());
            if (auto panel_locked = panel.lock()) {
                panel_locked->request_fit_to_screen();
            }
        }
    }
}

}

