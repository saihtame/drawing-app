#include "plane.hpp"
#include <glm/ext/vector_uint2.hpp>
#include <glad/gl.h>


namespace Infrastructure::Rendering {


const float vertex_data[] = {
//    X      Y     Z     UV x   UV y
    -0.5f,  0.5f, 0.0f, -1.0f,  1.0f, // Top Left
     0.5f,  0.5f, 0.0f,  1.0f,  1.0f, // Top Right
     0.5f, -0.5f, 0.0f,  1.0f, -1.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, -1.0f, -1.0f, // Bottom Left
};

const uint32_t indices[] = {
    0, 1, 2,
    2, 3, 0
};

Plane::Plane() {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UVs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3);
    glEnableVertexAttribArray(1);
}

Plane::~Plane() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Plane::render() {
    // Bind VAO
    glBindVertexArray(VAO);
    // Render
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}

}
