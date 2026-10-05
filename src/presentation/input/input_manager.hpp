#pragma once
#include "SDL3/SDL_events.h"
#include "application/documents/document_service.hpp"
#include "application/tools/toolbar.hpp"
#include "domain/documents/canvas.hpp"
#include "domain/tools/stroke_tool.hpp"
#include "domain/tools/tool.hpp"
#include "imgui.h"
#include "presentation/canvas_panels/canvas_panel_service.hpp"
#include <memory>


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
    std::shared_ptr<Domain::Documents::Canvas> active_drag_canvas;
    std::shared_ptr<Domain::Tools::StrokeTool> active_drag_tool;

private: // Helpers
    inline bool mouse_inside_canvas(ImVec2 canvas_size, ImVec2 canvas_pos);
    inline glm::vec2 get_canvas_pos(SDL_Event& mouse_event);
    inline bool on_mouse_button_down(SDL_Event& event);
    inline bool start_tool_stroke(SDL_Event& event, std::shared_ptr<Domain::Tools::Tool>& tool);
    inline bool start_tool_click(SDL_Event& event, std::shared_ptr<Domain::Tools::Tool>& tool);
    inline bool on_mouse_button_up(SDL_Event& event);
    inline bool on_mouse_movement(SDL_Event& event);
};

}
