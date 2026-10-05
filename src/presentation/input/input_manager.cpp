#include "input_manager.hpp"
#include "SDL3/SDL_events.h"
#include "domain/tools/stroke_tool.hpp"
#include "domain/tools/tool.hpp"
#include "imgui.h"
#include <iostream>
#include <stdexcept>


namespace Presentation::Input {

InputManager::InputManager(Application::Documents::DocumentService& doc_service,
                           Application::Tools::Toolbar& toolbar,
                           CanvasPanels::CanvasPanelService& canvas_panel_service)
 : doc_service(doc_service), toolbar(toolbar), canvas_panel_service(canvas_panel_service) {}


bool InputManager::process_event(SDL_Event& event) {
    // Ensure a visible canvas exists
    auto visible_canvas = canvas_panel_service.get_visible_panel();
    if (auto visible_canvas_locked = visible_canvas.lock()) {
        auto canvas_size = visible_canvas_locked->get_size();
        auto canvas_pos = visible_canvas_locked->get_pos();
        // Ensure mouse is inside canvas. Since we only care about tool use logic.
        if (!mouse_inside_canvas(canvas_size, canvas_pos))
            return false;
    } else {
        return false;
    }

    // Check for motion
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        return on_mouse_movement(event);
    }
    // Check for mouse clicks
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        // Only care about left clicks
        if (event.button.button != SDL_BUTTON_LEFT)
            return false;

        return on_mouse_button_down(event);
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        // Only care about left clicks
        if (event.button.button != SDL_BUTTON_LEFT)
            return false;

        return on_mouse_button_up(event);
    }
    return false;
}

inline bool InputManager::mouse_inside_canvas(ImVec2 canvas_size, ImVec2 canvas_pos) {
    // Query mouse pos, to see if we are within the canvas panel
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    return mouse_x >= canvas_pos.x &&
           mouse_x <= canvas_pos.x + canvas_size.x &&
           mouse_y >= canvas_pos.y &&
           mouse_y <= canvas_pos.y + canvas_size.y;
}

inline glm::vec2 InputManager::get_canvas_pos(SDL_Event& mouse_event) {
    if (active_drag_canvas == nullptr) return {0, 0};

    glm::vec2 screen_pos = {mouse_event.motion.x, mouse_event.motion.y};
    return active_drag_canvas->get_canvas_pos(screen_pos);
}

inline bool InputManager::on_mouse_button_down(SDL_Event& event) {
    // Get the active tool
    auto tool = toolbar.get_active_tool();
    if (tool == nullptr)
        return false;

    // Check tool usage type
    Domain::Tools::ToolUsage usage_type = tool->get_tool_usage();
    switch (usage_type) {
        case Domain::Tools::TOOL_USAGE_CLICK:
            return start_tool_click(event, tool);
        case Domain::Tools::TOOL_USAGE_STROKE:
            return start_tool_stroke(event, tool);
        default:
            std::cerr << "Unknown tool usage type" << std::endl;
            return false;
    }
}

inline bool InputManager::start_tool_stroke(SDL_Event& event, std::shared_ptr<Domain::Tools::Tool>& tool) {
    // Cast tool to stroke tool
    const auto stroke_tool = dynamic_pointer_cast<Domain::Tools::StrokeTool>(tool);
    if (stroke_tool == nullptr) {
        std::cerr << "Failed to cast tool to stroke tool." << std::endl;
        return false;
    }

    // Try to get a canvas to operate on
    auto active_document = doc_service.get_active_document();
    if (auto active_document_locked = active_document.lock()) {
        auto active_drag_canvas_weak = active_document_locked->get_canvas();
        if (auto active_drag_canvas_locked = active_drag_canvas_weak.lock()) {
            // Cache the active canvas
            active_drag_canvas = active_drag_canvas_locked;
        } else {
            return false;
        }
    } else {
        // No active document at the moment
        return false;
    }

    // Get mouse pos
    glm::vec2 canvas_pos = get_canvas_pos(event);

    stroke_tool->start_stroke(canvas_pos);

    // Stark stroke
    dragging = true;
    active_drag_tool = stroke_tool;

    return true;
}


inline bool InputManager::start_tool_click(SDL_Event& event, std::shared_ptr<Domain::Tools::Tool>& tool) {
    throw std::runtime_error("Not implemented");
}

inline bool InputManager::on_mouse_button_up(SDL_Event& event) {
    if (!dragging)
        return false;

    dragging = false;
    active_drag_tool->stop_stroke(get_canvas_pos(event));

    return true;
}

inline bool InputManager::on_mouse_movement(SDL_Event& event) {
    if (!dragging)
        return false;

    active_drag_tool->continue_stroke(get_canvas_pos(event));

    return true;
}

}
