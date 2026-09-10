#pragma once
#include <glm/vec2.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <string>


namespace Domain::Documents {

class CanvasLayer {
public: // Ctors
    CanvasLayer(std::string name, glm::uvec2 size) : layer_name(name), size(size) {}
    ~CanvasLayer() = default;

private: // Data
    std::string layer_name = "";
    glm::uvec2 size = {0, 0};
    glm::uvec2 offset = {0, 0};
};

}
