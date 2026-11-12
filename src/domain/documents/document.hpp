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

public: // Getters and setters
    /**
     * @brief Returns the document's canvas.
     * The canvas is created immediately in the constructor and never reset, so
     * this always returns a valid reference.
     */
    [[nodiscard]] Canvas& get_canvas() noexcept;
    [[nodiscard]] UUIDv4::UUID get_uuid() const noexcept { return uuid; }
    [[nodiscard]] std::optional<std::filesystem::path> get_save_path() { return save_path; }
    void set_save_path(std::filesystem::path path) { save_path = path; }

private: // Data
    UUIDv4::UUID uuid;
    std::optional<std::filesystem::path> save_path;
    std::shared_ptr<Canvas> canvas;
    glm::uvec2 size;
};

}
