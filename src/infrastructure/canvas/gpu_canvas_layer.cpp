#include "gpu_canvas_layer.hpp"
#include "domain/documents/canvas_layer.hpp"


namespace Infrastructure::Canvas {

GpuCanvasLayer::GpuCanvasLayer(std::string layer_name, glm::uvec2 size)
 : Domain::Documents::CanvasLayer(layer_name, size), buffers{size, size} {
}

GpuCanvasLayer::~GpuCanvasLayer() = default;

void GpuCanvasLayer::fill(glm::vec4 color) {
    // Both buffers have to match. Otherwise the untouched one resurfaces after
    // the first switch_buffers() and the fill appears to be lost.
    for (auto& buffer : buffers)
        buffer.fill(color);
}

}
