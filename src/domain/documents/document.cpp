#include "document.hpp"
#include <memory>


namespace Domain::Documents {

Document::Document(const CanvasLayerFactory& canvas_layer_factory, glm::uvec2 size)
 : canvas_layer_factory(canvas_layer_factory), size(size) {
    canvas = std::make_unique<Canvas>(canvas_layer_factory, size);
}

Canvas& Document::get_canvas() const noexcept {
    assert(canvas != nullptr);
    return *canvas;
}

}

