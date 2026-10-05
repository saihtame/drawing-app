#pragma once
#include <glm/vec4.hpp>


namespace Domain::Tools {

/**
 * @brief Configuration for a brush tool.
 * Owned by the tool itself (per-tool settings), not by a document or a
 * service. A Toolbar holds tools for the lifetime of the application, so
 * settings naturally survive switching away from the brush and back.
 */
struct BrushSettings {
    /**
     * @brief Diameter of the brush tip in pixels.
     */
    float size = 8.0f;
    /**
     * @brief Falloff of the brush tip. 0 is fully soft, 1 is fully hard.
     */
    float hardness = 0.5f;
    /**
     * @brief Opacity applied to each dab, in range [0, 1].
     */
    float opacity = 1.0f;
    /**
     * @brief Normalized RGBA color of the brush, components in range [0, 1].
     */
    glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f};
};

}
