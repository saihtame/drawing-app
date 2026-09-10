#include "gpu_canvas.hpp"
#include "gpu_canvas_layer.hpp"
#include "glad/gl.h"
#include <iostream>


namespace Infrastructure::Canvas {

GpuCanvas::GpuCanvas(glm::uvec2 size) : Canvas(size) {
}

void GpuCanvas::render(glm::vec2 position) {
    // Disable depth buffer
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    if (material == nullptr) {
        std::cerr << "Material null" << std::endl;
        return;
    }
    if (plane == nullptr) {
        std::cerr << "Plane is null" << std::endl;
        return;
    }

    material->use(position);
    plane->render();
}

int GpuCanvas::get_layer_count() const noexcept {
    return layers.size();
}

std::weak_ptr<Domain::Documents::CanvasLayer> GpuCanvas::get_layer(int ind) const {
    // Check index
    if (ind >= layers.size() || ind < 0)
        throw std::invalid_argument("Layer index invalid: " + std::to_string(ind));

    auto it = std::next(layers.begin(), ind);
    return *it;
}

void GpuCanvas::add_layer() {
    // Create layer
    std::string layer_name = "Layer " + std::to_string(layers.size() + 1);
    auto layer = std::make_shared<GpuCanvasLayer>(layer_name, size);

    // Add to list
    layers.push_back(std::move(layer));
}

void GpuCanvas::remove_layer(int ind) {
    // Check index
    if (ind >= layers.size() || ind < 0)
        throw std::invalid_argument("Layer index invalid: " + std::to_string(ind));

    auto it = std::next(layers.begin(), ind);
    layers.erase(it);
}

}
