#include "brush_painter.hpp"
#include "infrastructure/rendering/texture.hpp"


namespace Infrastructure::Compute {

BrushPainter::BrushPainter() {
    // Create shader program
    shader_program.add_shader(shader_path, GL_COMPUTE_SHADER);
    shader_program.link();
}

void BrushPainter::paint(glm::vec2 start, glm::vec2 end, Rendering::Texture& target, float radius) {
    shader_program.use();

    shader_program.set_uniform_float("radius", radius);
}

}
