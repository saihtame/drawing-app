#pragma once
#include "domain/documents/canvas_layer.hpp"
#include "infrastructure/rendering/texture.hpp"
#include <glm/ext/vector_uint2.hpp>


namespace Infrastructure::Canvas {

class GpuCanvasLayer final : public Domain::Documents::CanvasLayer {
public: // Ctors
    GpuCanvasLayer(std::string layer_name, glm::uvec2 size);
    ~GpuCanvasLayer();

public: // CanvasLayer implementation
    void fill(glm::vec4 color) override;

public: // Accessors
    /**
     * @brief The buffer holding the layer's current contents.
     * This is what gets composited when the canvas renders.
     */
    Rendering::Texture& get_texture() { return buffers[front]; }
    /**
     * @brief The buffer an edit should read from.
     */
    Rendering::Texture& get_read_texture() { return buffers[front]; }
    /**
     * @brief The buffer an edit should write to. Always a different texture
     * from get_read_texture().
     */
    Rendering::Texture& get_write_texture() { return buffers[front ^ 1]; }
    /**
     * @brief Promotes the write buffer to be the layer's current contents.
     * Call once a compute pass has finished writing into get_write_texture(),
     * otherwise the layer keeps showing the previous state.
     */
    void switch_buffers() noexcept { front ^= 1; }

private: // Data
    // GL cannot bind one texture as both a read and a write image within a
    // single dispatch, so edits must go through two buffers. Switching is an
    // index flip rather than a swap because Texture is not movable: it owns a
    // GL name and has a user declared destructor.
    Rendering::Texture buffers[2];
    int front = 0;
};

}
