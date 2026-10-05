#pragma once
#include "domain/tools/brush_settings.hpp"
#include "infrastructure/rendering/shader_program.hpp"
#include "infrastructure/rendering/texture.hpp"
#include <filesystem>
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Compute {

class BrushPainter final {
public: // Ctors
    BrushPainter();
    ~BrushPainter() = default;

public: // Usage
    /**
     * @brief Paints the stroke segment between two canvas space positions.
     *
     * Reads from source and writes to target, which must be two different
     * textures of equal size. GpuCanvasLayer is double buffered for exactly
     * this reason; the caller is responsible for promoting target afterwards,
     * with GpuCanvasLayer::switch_buffers().
     *
     * @param settings Brush configuration, already clamped by Domain::Tools::BrushTool.
     */
    void paint(glm::vec2 start, glm::vec2 end, const Rendering::Texture& source,
               Rendering::Texture& target, const Domain::Tools::BrushSettings& settings);

private: // Data
    Rendering::ShaderProgram shader_program;

private: // Constants
    const std::filesystem::path shader_path = "shaders/compute/brush_painter.comp";
    static constexpr glm::uvec2 WORK_GROUP_SIZE = {16, 16};
};

}
