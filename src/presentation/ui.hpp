#pragma once
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "application/documents/document_service.hpp"
#include "menu_bar.hpp"
#include "popups/popup_manager.hpp"


// ImGui forward declarations
class ImGuiViewport;

namespace Presentation {

class UI {
public:
    UI(Application::Documents::DocumentService& doc_service,
       SDL_Window* window, void* gl_context);
    ~UI();

public:
    bool processEvent(SDL_Event* event);
    void render();

private: // Members
    bool first_render = true;
    SDL_Window* window;
    // UI Components:
    Popups::PopupManager popup_manager;
    MenuBar menu_bar;

private: // Render methods
    inline void setup_first_render(uint32_t dockspace_id, ImGuiViewport* viewport);

private: // Setup methods
    inline void setup_style();

private: // Constants
    static constexpr const char* glsl_version = "#version 460";
};

}
