#include "document.hpp"
#include <stdexcept>


namespace Domain::Documents {

Document::Document(glm::uvec2 size) : size(size) {
}

int Document::get_layer_count() const noexcept {
    return layers.size();
}

Layer* Document::get_layer(int ind) const {
    // Check index
    if (ind > layers.size() || ind < 0)
        throw std::invalid_argument("Layer index invalid: " + std::to_string(ind));

    auto it = std::next(layers.begin(), ind);
    return it->get();
}

void Document::add_layer() {
    // Create layer
    std::unique_ptr<Layer> layer = std::make_unique<Layer>(size);

    // Add to list
    layers.push_back(std::move(layer));
}

void Document::remove_layer(int ind) {
    // Check index
    if (ind > layers.size() || ind < 0)
        throw std::invalid_argument("Layer index invalid: " + std::to_string(ind));

    auto it = std::next(layers.begin(), ind);
    layers.erase(it);
}

}

