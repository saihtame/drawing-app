#include "plane.hpp"
#include <memory>
#include <glad/gl.h>


namespace Infrastructure::Rendering {


const float vertex_data[] = {
    -0.5f, 0.5f,
    0.5f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
};

const int indices[] = {
    0, 1, 2,
    2, 3, 1
};

Plane::Plane() {
    // Prepare shader program
    shader_program = std::make_unique<ShaderProgram>();

    // Create VAO
    glGenVertexArrays(1, &VAO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Generate VBO
    glGenBuffers(1, &VBO);
    // Bind VBO buffer to the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Copy indices data to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Create and enable vertex attribues
    // Vertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Plane::~Plane() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Plane::render() {

}

}
