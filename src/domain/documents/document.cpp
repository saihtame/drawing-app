#include "document.hpp"
#include <memory>


namespace Domain::Documents {

Document::Document(const CanvasFactory& canvas_factory, glm::uvec2 size)
 : canvas_factory(canvas_factory), size(size) {
    // Create canvas
    canvas = canvas_factory.create_canvas(size);
    // Generate UUID
    UUIDv4::UUIDGenerator<std::mt19937_64> uuid_generator;
    uuid = uuid_generator.getUUID();
}

std::weak_ptr<Canvas> Document::get_canvas() const noexcept {
    return canvas;
}

}

