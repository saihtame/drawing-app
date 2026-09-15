#pragma once
#include <filesystem>
#include <memory>
#include <glm/ext/vector_int3.hpp>
#include <glm/vec2.hpp>
#include <optional>
#include "canvas.hpp"
#include "canvas_factory.hpp"
#include "uuid_v4.h"


namespace Domain::Documents {

class Document final {
public: // Ctors
    Document(const CanvasFactory& canvas_factory, glm::uvec2 size);
    ~Document() = default;

private: // Dependencies
    const CanvasFactory& canvas_factory;

public: // Getters
    /**
     * @brief Returns a weak_ptr to the canvas.
     * The canvas is created imediately, so this should always return a non-expired ptr.
     */
    [[nodiscard]] std::weak_ptr<Canvas> get_canvas() const noexcept;
    [[nodiscard]] UUIDv4::UUID get_uuid() const noexcept { return uuid; }

private: // Data
    UUIDv4::UUID uuid;
    std::optional<std::filesystem::path> save_path;
    std::shared_ptr<Canvas> canvas;
    glm::uvec2 size;
};

}
