#pragma once
#include "shader.hpp"
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <unordered_map>


namespace Infrastructure::Rendering {

class ShaderProgram final {
public:
    ShaderProgram();
    ~ShaderProgram();

    void use();

private: // Helpers
    inline void add_shader(std::filesystem::path path, GLint shader_type);
    inline void link();

private: // Data
    unsigned int program_id = 0;
    std::unordered_map<std::string, GLint> uniform_locations;
    std::vector<Shader> shaders;

private: // Constants
    const std::filesystem::path vertex_shader_path = "shaders/visual/canvas.vert";
    const std::filesystem::path fragment_shader_path = "shaders/visual/canvas.frag";
};

}
