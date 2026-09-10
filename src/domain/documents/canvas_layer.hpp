#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <string>


namespace Domain::Documents {

class CanvasLayer {
public: // Ctors
    CanvasLayer(std::string name, glm::uvec2 size) : layer_name(name), size(size) {}
    ~CanvasLayer() = default;

public:
    virtual void toggle_visiblity() { visible = !visible; }
    virtual void fill(glm::vec4 color) = 0;

public: // Accessors
    [[nodiscard]] const std::string& get_name() const noexcept { return layer_name; }
    [[nodiscard]] bool is_visible() const noexcept { return visible; }

protected: // Data
    std::string layer_name = "";
    glm::uvec2 size = {0, 0};
    glm::uvec2 offset = {0, 0};
    bool visible = true;
};

}
