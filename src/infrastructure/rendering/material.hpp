#pragma once
#include <filesystem>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>


namespace Infrastructure::Rendering {

class Material {
public:
    Material();
    ~Material() = default;

public:
    void use(glm::vec2 position);

private:
    glm::quat orientation;
    glm::vec3 scale {100.0, 100.0, 1.0};
    glm::mat4 transform;

private: // Constants
    const std::filesystem::path vertex_shader_path = "shaders/visual/canvas.vert";
    const std::filesystem::path fragment_shader_path = "shaders/visual/canvas.frag";
};

}
