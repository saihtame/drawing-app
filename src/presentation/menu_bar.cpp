#include "menu_bar.hpp"
#include "imgui.h"
#include "presentation/popups/new_file_popup.hpp"


namespace  Presentation {

MenuBar::MenuBar(Application::Documents::DocumentService& doc_service,
                 Popups::PopupManager& popup_manager)
 : doc_service(doc_service), popup_manager(popup_manager) {
}

void MenuBar::render() {
    if(ImGui::BeginMainMenuBar()) {
        // File menu
        if (ImGui::BeginMenu("File", true)) {
            // New File
            if (ImGui::MenuItem("New file")) {
                new_file();
            }
            // Load File
            if (ImGui::MenuItem("Load file")) {
                load_file();
            }
            bool can_save = doc_service.has_active_document();
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

}
