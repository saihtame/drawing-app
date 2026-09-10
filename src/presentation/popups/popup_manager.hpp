#pragma once
#include <memory>
#include <vector>
#include "popup.hpp"


namespace Presentation::Popups {

class PopupManager {
public:
    PopupManager() = default;
    ~PopupManager() = default;

public:
    bool is_popup_open() const;
    void push_popup(std::shared_ptr<Popup> popup);
    void render();

private: // Helpers
    inline void pop_popup();

private: // Members
    struct PopupEntry {
        std::shared_ptr<Popup> popup;
        bool needs_open = true;
    };
    std::vector<PopupEntry> popup_stack;
};

}
