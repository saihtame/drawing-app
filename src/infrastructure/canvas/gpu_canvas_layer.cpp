#include "gpu_canvas_layer.hpp"
#include "domain/documents/canvas_layer.hpp"
#include "glad/gl.h"


namespace Infrastructure::Canvas {

GpuCanvasLayer::GpuCanvasLayer(std::string layer_name, glm::uvec2 size)
 : Domain::Documents::CanvasLayer(layer_name, size) {
    glGenTextures(1, &texture);
}

void GpuCanvasLayer::render(uint32_t fbo) {
    // Add texture data to frame buffer
    return;
    glad_glFramebufferTexture(
        fbo,
        GL_COLOR_ATTACHMENT0,
        texture,
        0
    );
}

}
