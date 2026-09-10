#pragma once
#include "infrastructure/rendering/shader_program.hpp"
#include "infrastructure/rendering/texture.hpp"
#include <cstdint>
#include <filesystem>
#include <glm/ext/vector_uint2.hpp>
#include <vector>


namespace Infrastructure::Compute {

class TextureCompositor final {
public: // Ctors
    TextureCompositor();
    ~TextureCompositor() = default;

public:
    void composit(glm::uvec2 size, Rendering::Texture& target, const std::vector<uint32_t>& sources);

private: // Data
    Rendering::ShaderProgram shader_program;

private: // Constants
    const std::filesystem::path shader_path = "shaders/compute/texture_compositor.comp";
    static constexpr glm::uvec2 WORK_GROUP_SIZE = {16, 16};
    static constexpr int32_t MAX_SOURCES = 32;
};

}
