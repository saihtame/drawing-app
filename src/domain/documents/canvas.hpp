#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <memory>
#include "canvas_layer.hpp"


namespace Domain::Documents {

class Canvas {
public: // Ctors
    Canvas(glm::uvec2 size) : size(size) {}

public: // Rendering
    virtual void render(glm::vec2 position) = 0;

public: // Layer management
    virtual int get_layer_count() const noexcept = 0;
    virtual std::weak_ptr<CanvasLayer> get_layer(int ind) const = 0;
    virtual std::weak_ptr<CanvasLayer> add_layer() = 0;
    virtual void remove_layer(int ind) = 0;
    virtual void move_layer(int from, int to) = 0;

protected: // Data
    glm::uvec2 size = {0.0, 0.0};
};

}
