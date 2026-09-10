#include "texture.hpp"
#include "glad/gl.h"


namespace Infrastructure::Rendering {

Texture::Texture(glm::uvec2 size) {
    // Generate texture on GPU
    glGenTextures(1, &texture_id);
    // Set texture size
    glTextureStorage2D(
        texture_id,
        1,
        GL_RGBA16F,
        size.x,
        size.y
    );
    // Set parameters
    glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::~Texture() {
    // Delete texture from GPU memory
    glDeleteTextures(1, &texture_id);
}

}
