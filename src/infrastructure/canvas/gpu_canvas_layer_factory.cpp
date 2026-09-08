#include "gpu_canvas_layer_factory.hpp"
#include "gpu_canvas_layer.hpp"


namespace Infrastructure::Canvas {


std::shared_ptr<Domain::Documents::CanvasLayer> GpuCanvasLayerFactory::create_canvas_layer(std::string layer_name, glm::uvec2 size) const {
    auto layer = std::make_shared<GpuCanvasLayer>(layer_name, size);
    return layer;
}

}
