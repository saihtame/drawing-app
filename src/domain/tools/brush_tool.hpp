#pragma once
#include "brush_settings.hpp"
#include "stroke_tool.hpp"


namespace Domain::Tools {

/**
 * @brief Domain side of the brush tool.
 *
 * Owns the brush configuration and the rules for changing it. Concrete,
 * GPU backed implementations (see Infrastructure::Tools::BrushTool) derive
 * from this and read the settings when they render.
 */
class BrushTool : public StrokeTool {
public: // Settings accessors
    [[nodiscard]] const BrushSettings& get_settings() const noexcept { return settings; }

public: // Settings
    /**
     * @brief Sets the diameter of the brush tip.
     * Values outside the supported range are clamped.
     */
    void set_size(float value);
    /**
     * @brief Sets the falloff of the brush tip.
     * Values outside the supported range are clamped.
     */
    void set_hardness(float value);
    /**
     * @brief Sets the opacity applied to each dab.
     * Values outside the supported range are clamped.
     */
    void set_opacity(float value);
    /**
     * @brief Sets the brush color. Components are clamped to [0, 1].
     */
    void set_color(glm::vec4 value);

public: // Constants
    // Bounds live next to the clamping logic so that a UI cannot render
    // controls with ranges that disagree with what the setters accept.
    static constexpr float MIN_SIZE = 1.0f;
    static constexpr float MAX_SIZE = 512.0f;
    static constexpr float MIN_HARDNESS = 0.0f;
    static constexpr float MAX_HARDNESS = 1.0f;
    static constexpr float MIN_OPACITY = 0.0f;
    static constexpr float MAX_OPACITY = 1.0f;

protected: // Data
    BrushSettings settings;
};

}
