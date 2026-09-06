#include "popup_manager.hpp"
#include "imgui.h"
#include <iostream>
#include <stdexcept>


namespace Presentation::Popups {

void PopupManager::push_popup(std::shared_ptr<Popup> popup) {
    if (popup == nullptr) {
        throw std::invalid_argument("Null popup");
    }

    popup_stack.push_back({std::move(popup)});

#ifdef _DEBUG
    std::cout << "Opened popup " << popup_stack.back().popup->get_name() << std::endl;
#endif
}

void PopupManager::render() {
    if (popup_stack.empty()) return;

    PopupEntry& entry = popup_stack.back();
    if (entry.needs_open) {
        // The popup must be opened in the same window/ID-stack context where
        // BeginPopupModal() will be called, so this is deferred until render.
        ImGui::OpenPopup(entry.popup->get_name());
        entry.needs_open = false;
    }

    // Render top-most popup
    if (!entry.popup->render())
        // Pop from stack, if popup is closed etc.
        pop_popup();
}

void PopupManager::pop_popup() {
    if (popup_stack.empty()) {
        throw std::runtime_error("Cannot pop popup, when no popup is active.");
    }

    const char* popup_name = popup_stack.back().popup->get_name();
    popup_stack.pop_back();

#ifdef _DEBUG
    std::cout << "Closed popup " << popup_name << std::endl;
#endif

    // The newly exposed popup is no longer open in ImGui (one popup per
    // hierarchy level), so it must be reopened on the next frame.
    if (!popup_stack.empty()) {
        popup_stack.back().needs_open = true;
    }
}

}