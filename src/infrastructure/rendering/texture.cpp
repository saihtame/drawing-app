#include "texture.hpp"
#include "glad/gl.h"
#include <glm/gtc/type_ptr.hpp>


namespace Infrastructure::Rendering {

Texture::Texture(glm::uvec2 size, glm::vec4 fill_color) {
    // Generate texture on GPU
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
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

    fill(fill_color);
}

void Texture::fill(glm::vec4 color) {
    glClearTexImage(texture_id, 0, GL_RGBA, GL_FLOAT, glm::value_ptr(color));
}

Texture::~Texture() {
    // Delete texture from GPU memory
    glDeleteTextures(1, &texture_id);
}

}
