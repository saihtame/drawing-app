#include "texture_compositor.hpp"
#include "infrastructure/rendering/shader.hpp"
#include "infrastructure/rendering/texture.hpp"
#include <glm/ext/vector_uint2.hpp>
#include <stdexcept>


namespace Infrastructure::Compute {

TextureCompositor::TextureCompositor() {
    // Create shader program
    shader_program.add_shader(shader_path, GL_COMPUTE_SHADER);
    shader_program.link();
}

void TextureCompositor::composit(glm::uvec2 size, Rendering::Texture& target, const std::vector<uint32_t>& sources) {
    shader_program.use();

    // Check sources size
    int sources_size = sources.size();
    if (sources_size > MAX_SOURCES) {
        // TODO: Implement multiple passes for sources_size exceeding limit
        throw std::invalid_argument("Sources cannot be of size greater than " + std::to_string(MAX_SOURCES));
    }

    // Set parameters
    shader_program.set_uniform_int("source_count", sources_size);

    // Bind target
    glBindImageTexture(0, target.get_id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

    // Bind sources
    glBindTextures(1, sources_size, sources.data());

    // Dispatch shader program
    glm::uvec2 dispatches = {
        size.x / (WORK_GROUP_SIZE.x + 1),
        size.y / (WORK_GROUP_SIZE.y + 1)
    };
    glDispatchCompute(dispatches.x, dispatches.y, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

}
