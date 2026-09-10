#pragma once
#include "domain/documents/canvas_layer.hpp"
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Canvas {

class GpuCanvasLayer final : public Domain::Documents::CanvasLayer {
public: // Ctors
    GpuCanvasLayer(std::string layer_name, glm::uvec2 size);
    ~GpuCanvasLayer() = default;

public: // CanvasLayer implementation
    void render(uint32_t fbo);

private: // Data
    uint32_t texture;
};

}
