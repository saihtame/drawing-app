#pragma once
#include <filesystem>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>


namespace Infrastructure::Rendering {

class Material {
public:
    Material();
    ~Material() = default;

public:
    void use(glm::vec2 position, uint32_t texture_id);

private:
    glm::quat orientation = glm::identity<glm::quat>();
    glm::vec3 scale {200.0, 200.0, 1.0};

private: // Constants
    const std::filesystem::path vertex_shader_path = "shaders/visual/canvas.vert";
    const std::filesystem::path fragment_shader_path = "shaders/visual/canvas.frag";
};

}
