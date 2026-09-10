#include "document.hpp"
#include <memory>


namespace Domain::Documents {

Document::Document(const CanvasFactory& canvas_factory, glm::uvec2 size)
 : canvas_factory(canvas_factory), size(size) {
    canvas = canvas_factory.create_canvas(size);
}

std::weak_ptr<Canvas> Document::get_canvas() const noexcept {
    return canvas;
}

}

