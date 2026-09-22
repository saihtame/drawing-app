#pragma once
#include "SDL3/SDL_events.h"
#include "application/documents/document_service.hpp"
#include "application/tools/toolbar.hpp"
#include "imgui.h"
#include "presentation/canvas_panels/canvas_panel_service.hpp"


namespace Presentation::Input {

class InputManager final {
public: // Ctors
    InputManager(Application::Documents::DocumentService& doc_service,
                 Application::Tools::Toolbar& toolbar,
                 CanvasPanels::CanvasPanelService& canvas_panel_service);
    ~InputManager() = default;

public:
    bool process_event(SDL_Event& event);

private: // Dependencies
    Application::Documents::DocumentService& doc_service;
    Application::Tools::Toolbar& toolbar;
    CanvasPanels::CanvasPanelService& canvas_panel_service;

private: // Data
    bool dragging = false;

private: // Helpers
    inline bool mouse_inside_canvas(ImVec2 canvas_size, ImVec2 canvas_pos);
    inline bool on_mouse_button_down(SDL_Event& event);
    inline bool on_mouse_button_up(SDL_Event& event);
    inline bool on_mouse_movement(SDL_Event& event);
};

}
