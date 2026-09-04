#pragma once
#include <glm/ext/vector_int3.hpp>
#include <glm/vec2.hpp>
#include "layer.hpp"


namespace Domain {

class Document {
public:
    Document(glm::ivec2 size);
    ~Document() = default;

private:
    std::vector<Layer> layers;
};

}
