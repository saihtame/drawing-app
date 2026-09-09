#pragma once
#include "shader_program.hpp"
#include <memory>


namespace Infrastructure::Rendering {

class Plane final {
public:
    Plane();
    ~Plane();

public:
    void render();

private: // Buffer object ids
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

private: // Shader program
    std::unique_ptr<ShaderProgram> shader_program;

};

};
