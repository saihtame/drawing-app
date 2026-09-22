#include "input_manager.hpp"
#include "SDL3/SDL_events.h"
#include "imgui.h"
#include <iostream>


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

inline bool InputManager::on_mouse_button_down(SDL_Event& event) {
    dragging = true;
    std::cout << "Mouse click!" << std::endl;
    return false;
}

inline bool InputManager::on_mouse_button_up(SDL_Event& event) {
    if (!dragging)
        return false;
    std::cout << "Mouse release!" << std::endl;
    dragging = false;
    return false;
}

inline bool InputManager::on_mouse_movement(SDL_Event& event) {
    if (!dragging)
        return false;
    std::cout << "Mouse motion!" << std::endl;
    return false;
}

}
