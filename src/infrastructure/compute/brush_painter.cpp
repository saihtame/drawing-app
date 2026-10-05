#include "brush_painter.hpp"
#include "glad/gl.h"
#include "infrastructure/rendering/texture.hpp"
#include <stdexcept>


namespace Infrastructure::Compute {

BrushPainter::BrushPainter() {
    // Safe to compile here: BrushPainter is constructed lazily on the first
    // stroke, which happens inside the event loop once a GL context exists.
    shader_program.add_shader(shader_path, GL_COMPUTE_SHADER);
    shader_program.link();
}

void BrushPainter::paint(glm::vec2 start, glm::vec2 end, const Rendering::Texture& source,
                         Rendering::Texture& target, const Domain::Tools::BrushSettings& settings) {
    // Binding one texture to two image units is a feedback loop
    if (source.get_id() == target.get_id())
        throw std::invalid_argument("Brush source and target must be different textures");
    if (source.get_size() != target.get_size())
        throw std::invalid_argument("Brush source and target must have the same size");

    shader_program.use();

    // Set parameters. The shader works in radius, the settings in tip diameter.
    shader_program.set_uniform_float("radius", settings.size * 0.5f);
    shader_program.set_uniform_float("hardness", settings.hardness);
    shader_program.set_uniform_float("opacity", settings.opacity);
    shader_program.set_uniform_vec4("color", settings.color);
    shader_program.set_uniform_vec2("start", start);
    shader_program.set_uniform_vec2("end", end);

    // Bind the destination for writing and the source for reading
    glBindImageTexture(0, target.get_id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
    glBindImageTexture(1, source.get_id(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);

    // Dispatch, rounding up so that the edges of the canvas are covered too
    glm::uvec2 size = target.get_size();
    glm::uvec2 dispatches = {
        (size.x + WORK_GROUP_SIZE.x - 1u) / WORK_GROUP_SIZE.x,
        (size.y + WORK_GROUP_SIZE.y - 1u) / WORK_GROUP_SIZE.y,
    };
    glDispatchCompute(dispatches.x, dispatches.y, 1);

    // Two kinds of reader follow this dispatch: the compositor reads layers as
    // samplers in the next frame, and the next dab reads this texture through an
    // image unit. The image bit alone does not cover the sampler case.
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

}