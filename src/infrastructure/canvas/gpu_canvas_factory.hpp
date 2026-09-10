#pragma once
#include <memory>
#include "domain/documents/canvas_factory.hpp"


namespace Infrastructure::Canvas {

class GpuCanvasFactory final : public Domain::Documents::CanvasFactory {
public: // Ctors
    GpuCanvasFactory() = default;
    ~GpuCanvasFactory() = default;

public: // CanvasFactory implementation
    std::shared_ptr<Domain::Documents::Canvas> create_canvas(glm::uvec2 size) const override;
};
}
