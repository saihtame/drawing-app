#pragma once
#include <filesystem>
#include <memory>
#include <glm/ext/vector_int3.hpp>
#include <glm/vec2.hpp>
#include <optional>
#include "canvas.hpp"
#include "canvas_layer_factory.hpp"


namespace Domain::Documents {

class Document final {
public: // Ctors
    Document(const CanvasLayerFactory& canvas_layer_factory, glm::uvec2 size);
    ~Document() = default;

private: // Dependencies
    const CanvasLayerFactory& canvas_layer_factory;

private: // Data
    std::optional<std::filesystem::path> save_path;
    std::shared_ptr<Canvas> canvas;
    glm::uvec2 size;
};

}
