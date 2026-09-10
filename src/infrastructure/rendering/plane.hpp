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

private: // Buffer object ids
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};

};
