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

public:
    void add_shader(std::filesystem::path path, GLint shader_type);
    void link();
    void set_uniform_matrix(std::string uniform_name, glm::mat4 value);
    void set_uniform_vec2(std::string uniform_name, glm::vec2 value);
    void set_uniform_vec4(std::string uniform_name, glm::vec4 value);

private: // Helpers
    inline unsigned int get_uniform_location(std::string uniform_name);

private: // Data
    unsigned int program_id = 0;
    std::unordered_map<std::string, GLint> uniform_locations;
    std::vector<std::unique_ptr<Shader>> shaders;
};

}
