#include "canvas.hpp"


namespace Domain::Documents {

Canvas::Canvas(const CanvasLayerFactory& layer_factory, glm::uvec2 size)
: layer_factory(layer_factory), size(size) {
}

void Canvas::render(glm::ivec2 render_size) {
    // Render each layer of the canvas
    for (const auto& layer : layers) {
        layer->render();
    }
}

int Canvas::get_layer_count() const noexcept {
    return layers.size();
}

std::weak_ptr<CanvasLayer> Canvas::get_layer(int ind) const {
    // Check index
    if (ind >= layers.size() || ind < 0)
        throw std::invalid_argument("Layer index invalid: " + std::to_string(ind));

    auto it = std::next(layers.begin(), ind);
    return *it;
}

void Canvas::add_layer() {
    // Create layer
    std::string layer_name = "Layer " + std::to_string(layers.size() + 1);
    auto layer = layer_factory.create_canvas_layer(layer_name, size);

    // Add to list
    layers.push_back(std::move(layer));
}

void Canvas::remove_layer(int ind) {
    // Check index
    if (ind >= layers.size() || ind < 0)
        throw std::invalid_argument("Layer index invalid: " + std::to_string(ind));

    auto it = std::next(layers.begin(), ind);
    layers.erase(it);
}

}
