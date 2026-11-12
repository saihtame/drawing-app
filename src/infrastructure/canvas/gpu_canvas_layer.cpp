#include "gpu_canvas_layer.hpp"
#include "domain/documents/canvas_layer.hpp"
#include "glad/gl.h"
#include <stdexcept>


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

void GpuCanvasLayer::upload_pixels(std::span<const std::uint8_t> rgba) {
    const size_t expected = static_cast<size_t>(size.x) * size.y * 4;
    if (rgba.size() != expected)
        throw std::invalid_argument("Pixel buffer does not match layer size");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // rows are tightly packed
    // Both buffers have to match, for the same reason as fill().
    for (auto& buffer : buffers)
        glTextureSubImage2D(buffer.get_id(), 0, 0, 0,
                            static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y),
                            GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
}

}
