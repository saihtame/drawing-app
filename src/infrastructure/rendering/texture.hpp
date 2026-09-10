#pragma once
#include <cstdint>
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Rendering {

class Texture {
public: // Ctors
    Texture(glm::uvec2 size);
    ~Texture();

public:
    uint32_t get_id() const { return texture_id; }

private: // Data
    uint32_t texture_id = 0;
};

}
