#include "gpu_canvas_layer.hpp"
#include "domain/documents/canvas_layer.hpp"


namespace Infrastructure::Canvas {

GpuCanvasLayer::GpuCanvasLayer(std::string layer_name, glm::uvec2 size)
 : Domain::Documents::CanvasLayer(layer_name, size), texture(size) {
}

GpuCanvasLayer::~GpuCanvasLayer() {
}

void GpuCanvasLayer::fill(glm::vec4 color) {
    texture.fill(color);
}

}
