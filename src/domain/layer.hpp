#pragma once
#include <glm/ext/vector_int2.hpp>



namespace Domain {

class Layer {
public:
    Layer(glm::ivec2 size);
    ~Layer() = default;

private:
    glm::ivec2 size;
};

}
