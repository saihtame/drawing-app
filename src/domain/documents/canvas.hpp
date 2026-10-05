#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <memory>
#include "canvas_layer.hpp"
#include "uuid_v4.h"


namespace Domain::Documents {

class Canvas {
public: // Ctors
    Canvas(glm::uvec2 size) : size(size) {}

public: // Rendering
    /**
     * @brief Render the canvas to the screen
     *
     * @param position The center position of where on the screen the canvas should be rendered.
     */
    virtual void render(glm::vec2 position) = 0;

public: // Layer management
    virtual int get_layer_count() const noexcept = 0;
    virtual std::weak_ptr<CanvasLayer> get_layer(int ind) const = 0;
    virtual std::weak_ptr<CanvasLayer> add_layer() = 0;
    virtual void remove_layer(int ind) = 0;
    virtual void move_layer(int from, int to) = 0;
    virtual std::weak_ptr<CanvasLayer> get_selected_layer() const = 0;
    virtual void set_selected_layer(UUIDv4::UUID layer) = 0;

public: // Rendering
    /**
     * @brief Set the zoom level of the canvas when rendering.
     */
    virtual void set_zoom(float value) = 0;
    /**
     * @brief Changes the zoom level to fit the canvas within the screen size
     */
    virtual void fit_screen(glm::vec2 screen_size) = 0;
    /**
     * @brief Convert a position from screen space to canvas space
     */
    virtual glm::vec2 get_canvas_pos(glm::vec2 screen_pos) const = 0;

protected: // Data
    glm::uvec2 size = {0.0, 0.0};
};

}
