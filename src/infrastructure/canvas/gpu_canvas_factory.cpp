#include "gpu_canvas_factory.hpp"
#include "gpu_canvas.hpp"


namespace Infrastructure::Canvas {


std::shared_ptr<Domain::Documents::Canvas> GpuCanvasFactory::create_canvas(glm::uvec2 size) const  {
    auto layer = std::make_shared<GpuCanvas>(size);
    return layer;
}

}
