#pragma once
#include "domain/documents/canvas.hpp"
#include "gpu_canvas_layer.hpp"
#include "infrastructure/rendering/material.hpp"
#include "infrastructure/rendering/plane.hpp"
#include <glm/ext/vector_uint2.hpp>
#include <list>
#include <memory>


namespace Infrastructure::Canvas {

class GpuCanvas final : public Domain::Documents::Canvas {
public: // Ctors
    GpuCanvas(glm::uvec2 size);
    ~GpuCanvas() = default;

public: // Canvas implementation
    void render(glm::vec2 position) override;
    [[nodiscard]] int get_layer_count() const noexcept override;
    [[nodiscard]] std::weak_ptr<Domain::Documents::CanvasLayer> get_layer(int ind) const override;
    void add_layer() override;
    void remove_layer(int ind) override;
    void move_layer(int from, int to) override;

private: // Data
    std::list<std::shared_ptr<GpuCanvasLayer>> layers;
    std::unique_ptr<Rendering::Material> material = std::make_unique<Rendering::Material>();
    std::unique_ptr<Rendering::Plane> plane = std::make_unique<Rendering::Plane>();
};

}
