#pragma once
#include "application/documents/document_service.hpp"
#include "presentation/menu_bar.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>


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
    MenuBar menu_bar;

private: // Render methods
    inline void setup_first_render(uint32_t dockspace_id, ImGuiViewport* viewport);

private: // Setup methods
    inline void setup_style();

private: // Constants
    static constexpr const char* glsl_version = "#version 460";
};

}
