#include "material.hpp"
#include "infrastructure/rendering/shader.hpp"
#include "infrastructure/rendering/shader_program.hpp"
#include <glad/gl.h>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>


namespace Infrastructure::Rendering {

// Share shader program across all materials
static std::shared_ptr<ShaderProgram> shader_program;

Material::Material() {
    // Initialize shader program
    if (shader_program == nullptr) {
        shader_program = std::make_unique<ShaderProgram>();
        shader_program->add_shader(vertex_shader_path, GL_VERTEX_SHADER);
        shader_program->add_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
        shader_program->link();
    }
}

void Material::use(glm::vec2 position, uint32_t texture_id) {
    if (shader_program == nullptr) {
        std::cerr << "Shader program null" << std::endl;
        return;
    }

    // Query framebuffer size from the viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    if (width == 0 || height == 0) {
        std::cerr << "0 height or width" << std::endl;
    }

    // Pixel space to screen space projection (y-down, top-left origin)
    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f);

    // Calculate transform
    glm::vec3 world_pos = {position.x, position.y, 0.0f};
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), world_pos) // Position
                        * glm::mat4_cast(orientation);               // Rotation
    transform =           glm::scale(transform, scale);              // Scale

    // Setup shader program
    shader_program->use();
    shader_program->set_uniform_matrix("projection", projection);
    shader_program->set_uniform_matrix("transform", transform);
    // Bind image data
    glBindTextureUnit(0, texture_id);
}

}
