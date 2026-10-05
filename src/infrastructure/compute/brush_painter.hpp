#pragma once
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
    void paint(glm::vec2 start, glm::vec2 end, Rendering::Texture& target, float radius);

private: // Data
    Rendering::ShaderProgram shader_program;

private: // Constants
    const std::filesystem::path shader_path = "shaders/compute/brush_painter.comp";
    static constexpr glm::uvec2 WORK_GROUP_SIZE = {16, 16};
};

}
