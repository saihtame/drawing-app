#pragma once
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Rendering {

class Material;

class Plane final {
public:
    Plane();
    ~Plane();

public:
    void render();

public: // Buffer object ids
    unsigned int get_vao() const { return VAO; }
    unsigned int get_vbo() const { return VBO; }
    unsigned int get_ebo() const { return EBO; }

private: // Buffer object ids
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};

};
