#pragma once
#include <glm/ext/vector_float2.hpp>
#include <list>
#include <memory>
#include "canvas_layer.hpp"
#include "domain/documents/canvas_layer_factory.hpp"


namespace Domain::Documents {

class Canvas final {
public: // Ctors
    Canvas(const CanvasLayerFactory& layer_factory, glm::uvec2 size);
    ~Canvas() = default;

public: // Rendering
    void render(glm::vec2 render_size);

public: // Layer management
    int get_layer_count() const noexcept;
    [[nodiscard]] std::weak_ptr<CanvasLayer> get_layer(int ind) const;
    void add_layer();
    void remove_layer(int ind);

private: // Dependencies
    const CanvasLayerFactory& layer_factory;

private: // Data
    glm::uvec2 size;
    std::list<std::shared_ptr<CanvasLayer>> layers;
};

}
