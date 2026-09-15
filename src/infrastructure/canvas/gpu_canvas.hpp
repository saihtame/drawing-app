#pragma once
#include "domain/documents/canvas.hpp"
#include "gpu_canvas_layer.hpp"
#include "infrastructure/rendering/material.hpp"
#include "infrastructure/rendering/plane.hpp"
#include "infrastructure/rendering/texture.hpp"
#include "infrastructure/compute/texture_compositor.hpp"
#include "uuid_v4.h"
#include <glm/ext/vector_uint2.hpp>
#include <list>
#include <memory>


namespace Infrastructure::Canvas {

class GpuCanvas final : public Domain::Documents::Canvas {
public: // Ctors
    GpuCanvas(glm::uvec2 size);
    ~GpuCanvas() = default;

/* Canvas Implementation */
public: // Render
    void render(glm::vec2 position) override;
public: // Layer management
    [[nodiscard]] int get_layer_count() const noexcept override;
    [[nodiscard]] std::weak_ptr<Domain::Documents::CanvasLayer> get_layer(int ind) const override;
    std::weak_ptr<Domain::Documents::CanvasLayer> add_layer() override;
    void remove_layer(int ind) override;
    void move_layer(int from, int to) override;
public: // Selected layer
    std::weak_ptr<Domain::Documents::CanvasLayer> get_selected_layer() const override;
    void set_selected_layer(UUIDv4::UUID layer) override;
public: // Zoom
    void set_zoom(float value) override;
    void fit_screen(glm::vec2 screen_size) override;

private: // Helpers
    std::weak_ptr<GpuCanvasLayer> get_gpu_layer(UUIDv4::UUID uuid);

private: // Data
    Rendering::Texture texture;
    Rendering::Material material;
    Rendering::Plane plane;
    std::list<std::shared_ptr<GpuCanvasLayer>> layers;
    Compute::TextureCompositor compositor;
    std::weak_ptr<GpuCanvasLayer> selected_layer;
};

}
