#include "menu_bar.hpp"
#include "imgui.h"


namespace  Presentation {

MenuBar::MenuBar(Application::Documents::DocumentService& doc_service)
 : doc_service(doc_service) {
}

void MenuBar::render() {
    if(ImGui::BeginMainMenuBar()) {
        // File menu
        if (ImGui::BeginMenu("File", true)) {
            // New File
            if (ImGui::MenuItem("New file")) {
            }
            // Load File
            if (ImGui::MenuItem("Load file")) {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

}

