#include "shader_program.hpp"
#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/gl.h>


namespace Infrastructure::Rendering {

ShaderProgram::ShaderProgram() {
    program_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    if (program_id != 0)
        glDeleteProgram(program_id);
}

void ShaderProgram::add_shader(std::filesystem::path path, GLint shader_type) {
    shaders.push_back(std::make_unique<Shader>(path, shader_type));
    GLuint shader_id = shaders[shaders.size() - 1]->get_id();
    glAttachShader(program_id, shader_id);
}

void ShaderProgram::link() {
    int success;
    char info_log[512];
    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        std::cerr << "Could not link shader program\n" << info_log << std::endl;
    }
}

void ShaderProgram::use() {
    glUseProgram(program_id);
}

void ShaderProgram::set_uniform_matrix(std::string uniform_name, glm::mat4 matrix) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::set_uniform_vec2(std::string uniform_name, glm::vec2 value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform2f(loc, value.x, value.y);
}

void ShaderProgram::set_uniform_vec4(std::string uniform_name, glm::vec4 value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void ShaderProgram::set_uniform_int(std::string uniform_name, int32_t value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform1i(loc, value);
}

void ShaderProgram::set_uniform_float(std::string uniform_name, float value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform1f(loc, value);
}

inline unsigned int ShaderProgram::get_uniform_location(std::string uniform_name) {
    auto it = uniform_locations.find(uniform_name);
    if (it != uniform_locations.end())
        return it->second;

    unsigned int loc = glGetUniformLocation(program_id, uniform_name.c_str());
    uniform_locations[uniform_name] = loc;
    return loc;
}

}
