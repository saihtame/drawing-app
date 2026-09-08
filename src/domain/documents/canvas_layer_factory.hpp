#pragma once
#include <glm/ext/vector_uint2.hpp>
#include <memory>



namespace Domain::Documents {

// Forward declerations
class Document;
class Canvas;
class CanvasLayer;

class CanvasLayerFactory {
public:
    virtual std::shared_ptr<CanvasLayer> create_canvas_layer(std::string layer_name, glm::uvec2 size) const = 0;
};

}
