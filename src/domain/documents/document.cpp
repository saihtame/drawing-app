#include "document.hpp"
#include <memory>


namespace Domain::Documents {

Document::Document(const CanvasLayerFactory& canvas_layer_factory, glm::uvec2 size)
 : canvas_layer_factory(canvas_layer_factory), size(size) {
    canvas = std::make_shared<Canvas>(canvas_layer_factory, size);
}

}

