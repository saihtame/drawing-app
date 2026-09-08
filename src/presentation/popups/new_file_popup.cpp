#include "new_file_popup.hpp"
#include "imgui.h"
#include <algorithm>


namespace Presentation::Popups {

NewFilePopup::NewFilePopup( Application::Documents::DocumentService& doc_service)
    : doc_service(doc_service) {
}

bool NewFilePopup::render() {
    if (!ImGui::BeginPopupModal(get_name(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        return false;

    // Input fields operate on signed values; clamp when committing
    int width = size.x;
    int height = size.y;
    ImGui::InputInt("Width", &width);
    ImGui::InputInt("Height", &height);
    size.x = std::max(1, width);
    size.y = std::max(1, height);

    if (ImGui::Button("Create")) {
        doc_service.new_document(size);
        ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
    return true;
}

}
