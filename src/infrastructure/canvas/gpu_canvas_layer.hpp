#pragma once
#include "domain/documents/canvas_layer.hpp"
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Canvas {

class GpuCanvasLayer final : public Domain::Documents::CanvasLayer {
public: // Ctors
    GpuCanvasLayer(std::string layer_name, glm::uvec2 size);
    ~GpuCanvasLayer() = default;

public: // CanvasLayer implementation
    void render(glm::vec2 render_size) override;

private: // Helpers
    inline void resize(glm::vec2 new_size);

private: // Data
    glm::vec2 render_size = {0, 0};
};

}
