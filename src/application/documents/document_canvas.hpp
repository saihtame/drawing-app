#pragma once


#include <glm/ext/vector_int2.hpp>
namespace Application::Documents {

class DocumentCanvas {
public:
    virtual void render(glm::ivec2 render_size) = 0;
};

}
