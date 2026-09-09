#include "shader_program.hpp"
#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/gl.h>


namespace Infrastructure::Rendering {

ShaderProgram::ShaderProgram() {
    program_id = glCreateProgram();

    add_shader(vertex_shader_path, GL_VERTEX_SHADER);
    add_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
    link();
}

ShaderProgram::~ShaderProgram() {
    if (program_id != 0)
        glDeleteProgram(program_id);
}

inline void ShaderProgram::add_shader(std::filesystem::path path, GLint shader_type) {
    shaders.push_back(Shader(path, shader_type));
    GLuint shader_id = shaders[shaders.size() - 1].get_id();
    glAttachShader(program_id, shader_id);
}

inline void ShaderProgram::link() {
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

}
