#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <memory>



namespace Domain::Documents {

// Forward declerations
class Document;
class Canvas;

class CanvasFactory {
public:
    virtual std::shared_ptr<Canvas> create_canvas(glm::uvec2 size) const = 0;
};

}
