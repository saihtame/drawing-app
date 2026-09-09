#pragma once
#include "domain/documents/canvas_layer_factory.hpp"


namespace Infrastructure::Canvas {

class GpuCanvasLayerFactory final : public Domain::Documents::CanvasLayerFactory {
public: // CanvasLayerFactory implementation
    std::shared_ptr<Domain::Documents::CanvasLayer> create_canvas_layer(std::string layer_name, glm::uvec2 size) const override;
};
}
