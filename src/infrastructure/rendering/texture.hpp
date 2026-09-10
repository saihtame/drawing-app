#pragma once
#include <cstdint>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Rendering {

class Texture {
public: // Ctors
    Texture(glm::uvec2 size, glm::vec4 fill_color={0.0, 0.0, 0.0, 0.0});
    ~Texture();

public:
    uint32_t get_id() const { return texture_id; }
    void fill(glm::vec4 color);

private: // Data
    uint32_t texture_id = 0;
};

}
