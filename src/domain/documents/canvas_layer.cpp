#include "canvas_layer.hpp"


namespace Domain::Documents {

CanvasLayer::CanvasLayer(std::string name, glm::uvec2 size) : layer_name(name), size(size) {
    // Generate UUID
    UUIDv4::UUIDGenerator<std::mt19937_64> uuid_generator;
    uuid = uuid_generator.getUUID();
}

}
