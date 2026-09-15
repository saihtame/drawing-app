#include "gpu_canvas.hpp"
#include "domain/documents/canvas_layer.hpp"
#include "gpu_canvas_layer.hpp"
#include "glad/gl.h"
#include "infrastructure/rendering/material.hpp"
#include "infrastructure/rendering/texture.hpp"
#include <glm/common.hpp>
#include <memory>
#include <stdexcept>


namespace Infrastructure::Canvas {

GpuCanvas::GpuCanvas(glm::uvec2 size) : Canvas(size), texture(size), material(size) {
}

void GpuCanvas::render(glm::vec2 position) {
    // Disable depth buffer, culling and blending for the 2D canvas pass
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    // Disable face culling
    glDisable(GL_CULL_FACE);
    // Disable alpha blending
    glDisable(GL_BLEND);

    std::vector<uint32_t> layer_tex_ids;
    layer_tex_ids.reserve(layers.size());
    for (const auto& layer : layers) {
        layer_tex_ids.push_back(layer->get_texture().get_id());
    }
    compositor.composit(size, texture, layer_tex_ids);

    material.use(position, texture.get_id());
    plane.render();
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

std::weak_ptr<Domain::Documents::CanvasLayer> GpuCanvas::add_layer() {
    // Create layer
    std::string layer_name = "Layer " + std::to_string(layers.size() + 1);
    auto layer = std::make_shared<GpuCanvasLayer>(layer_name, size);

    // Add to list
    layers.push_back(layer);

    return layer;
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

std::weak_ptr<Domain::Documents::CanvasLayer> GpuCanvas::get_selected_layer() const {
    return selected_layer;
}

void GpuCanvas::set_selected_layer(UUIDv4::UUID layer_id) {
    auto layer = get_gpu_layer(layer_id);
}

void GpuCanvas::set_zoom(float value) {
    material.set_zoom(value);
}

void GpuCanvas::fit_screen(glm::vec2 screen_size) {
    float max_width =  screen_size.x / size.x;
    float max_height = screen_size.y / size.y;

    float scale = glm::min(max_width, max_height);
    material.set_zoom(scale);
}

std::weak_ptr<GpuCanvasLayer> GpuCanvas::get_gpu_layer(UUIDv4::UUID uuid) {
    for (const auto& layer : layers) {
        if (layer->get_uuid() == uuid)
            return layer;
    }
    // Not found
    return std::weak_ptr<GpuCanvasLayer>();
}

}
