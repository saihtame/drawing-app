#pragma once
#include "application/documents/document_service.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>


// ImGui forward declarations
class ImGuiViewport;

namespace Presentation {

class UI {
public:
    UI(std::shared_ptr<Application::Documents::DocumentService> doc_service,
       SDL_Window* window, void* gl_context);
    ~UI();

public:
    bool processEvent(SDL_Event* event);
    void render();

private: // Dependencies
    std::shared_ptr<Application::Documents::DocumentService> doc_service = nullptr;

private: // Members
    bool first_render = true;
    SDL_Window* window;

private: // Render methods
    inline void setup_first_render(uint32_t dockspace_id, ImGuiViewport* viewport);

private: // Setup methods
    inline void setup_style();

private: // Constants
    static constexpr const char* glsl_version = "#version 460";
};

}
