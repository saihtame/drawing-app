#pragma once
#include "domain/documents/canvas_layer.hpp"
#include "infrastructure/rendering/texture.hpp"
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Canvas {

class GpuCanvasLayer final : public Domain::Documents::CanvasLayer {
public: // Ctors
    GpuCanvasLayer(std::string layer_name, glm::uvec2 size);
    ~GpuCanvasLayer();

public: // Accessors
    Rendering::Texture& get_texture() { return texture; }

private: // Data
    Rendering::Texture texture;
};

}
