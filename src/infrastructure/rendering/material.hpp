#pragma once
#include <filesystem>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>


namespace Infrastructure::Rendering {

class Material {
public:
    Material(glm::vec2 size);
    ~Material() = default;

public:
    void use(glm::vec2 position, uint32_t texture_id);

public: // Getters and setters
    void set_size(glm::vec2 value);
    glm::vec2 get_size() const { return size; }
    void set_scale(float value);
    float get_scale() const { return scale_factor; }

private:
    glm::quat orientation = glm::identity<glm::quat>();
    // Size in pixels of the canvas we are representing
    glm::vec2 size {1.0, 1.0};
    // Scale
    float scale_factor = 1.0;
    // Size in pixels, in which the canvas will be rendered
    glm::vec3 scale {1.0, 1.0, 1.0};

private: // helpers
    inline glm::mat4 get_transform(glm::uvec2 position) const;
    inline void update_scale();

private: // Constants
    const std::filesystem::path vertex_shader_path = "shaders/visual/canvas.vert";
    const std::filesystem::path fragment_shader_path = "shaders/visual/canvas.frag";
};

}
