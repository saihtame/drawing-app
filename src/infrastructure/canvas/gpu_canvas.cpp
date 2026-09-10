#include "gpu_canvas.hpp"
#include "gpu_canvas_layer.hpp"
#include "glad/gl.h"
#include <iostream>
#include <stdexcept>


namespace Infrastructure::Canvas {

GpuCanvas::GpuCanvas(glm::uvec2 size) : Canvas(size) {
}

void GpuCanvas::render(glm::vec2 position) {
    // Disable depth buffer, culling and blending for the 2D canvas pass
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    // Disable face culling
    glDisable(GL_CULL_FACE);
    // Disable alpha blending
    glDisable(GL_BLEND);

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

void GpuCanvas::move_layer(int from, int to) {
    int layer_count = layers.size();
    if (from >= layer_count || from < 0) {
        throw std::invalid_argument("From index out of bounds");
    }
    if (to >= layer_count || to < 0) {
        throw std::invalid_argument("To index out of bounds");
    }
    if (from == to) return;

    auto from_it = std::next(layers.begin(), from);
    auto to_it = std::next(layers.begin(), to);

    if (from < to) {
        ++to_it;
    }

    layers.splice(to_it, layers, from_it);
}

}
