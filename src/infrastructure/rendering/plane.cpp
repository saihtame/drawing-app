#include "plane.hpp"
#include <glm/ext/vector_uint2.hpp>
#include <glad/gl.h>


namespace Infrastructure::Rendering {


// UVs span 0..1, not -1..1. The textures use GL_CLAMP_TO_EDGE, so negative
// UVs clamp the entire left half and top half of the quad to a single texel
// column and row, and squeeze the rest into the opposite corner.
//
// The V coordinate is flipped on purpose. Material's projection is
// glm::ortho(0, width, height, 0), which is y-flipped, so a vertex at local
// y = +0.5 ends up at the BOTTOM of the drawn quad. Giving it v = 0 is what
// keeps canvas row 0 at the visual top, which in turn keeps
// GpuCanvas::get_canvas_pos consistent with the brush's canvas space.
const float vertex_data[] = {
//    X      Y     Z     UV x   UV y
    -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, // Top Left (visually bottom, y-flipped)
     0.5f,  0.5f, 0.0f,  1.0f,  1.0f, // Top Right
     0.5f, -0.5f, 0.0f,  1.0f,  0.0f, // Bottom Right (visually top)
    -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, // Bottom Left
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
    // The offset argument is in BYTES, so the UV offset has to be scaled.
    // Passing (void*)3 would point 3 bytes into the vertex, i.e. into the
    // middle of aPos, and the shader would read a misaligned vec2.
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UVs: byte offset of the 4th float (x, y, z, u, v)
    constexpr size_t uv_offset = 3 * sizeof(float);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)uv_offset);
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
