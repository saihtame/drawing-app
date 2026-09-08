#include "gpu_canvas_layer.hpp"
#include "domain/documents/canvas_layer.hpp"


namespace Infrastructure::Canvas {

GpuCanvasLayer::GpuCanvasLayer(std::string layer_name, glm::uvec2 size)
 : Domain::Documents::CanvasLayer(layer_name, size) {

}

void GpuCanvasLayer::render(glm::uvec2 render_size) {
    if (this->render_size != render_size) {
        resize(render_size);
    }
}

inline void GpuCanvasLayer::resize(glm::uvec2 new_size) {
    render_size = new_size;
}

}
