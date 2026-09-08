#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <string>


namespace Domain::Documents {

class Layer final {
public:
    Layer(glm::uvec2 size);
    ~Layer() = default;

private:
    std::string layer_name;
    glm::uvec2 size;
    glm::uvec2 offset = glm::uvec2(0, 0);
};

}
