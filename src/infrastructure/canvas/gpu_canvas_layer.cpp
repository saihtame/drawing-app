#include "gpu_canvas_layer.hpp"
#include "domain/documents/canvas_layer.hpp"


namespace Infrastructure::Canvas {

GpuCanvasLayer::GpuCanvasLayer(std::string layer_name, glm::uvec2 size)
 : Domain::Documents::CanvasLayer(layer_name, size) {

}

void GpuCanvasLayer::render(glm::vec2 render_size) {
    if (this->render_size != render_size) {
        resize(render_size);
    }
}

inline void GpuCanvasLayer::resize(glm::vec2 new_size) {
    render_size = new_size;
}

}
