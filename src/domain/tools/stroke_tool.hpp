#pragma once
#include "domain/tools/tool.hpp"
#include <glm/ext/vector_float2.hpp>


namespace Domain::Tools {

class StrokeTool : public Tool {
public: // Tool implementation
    ToolUsage get_tool_usage() const { return TOOL_USAGE_STROKE; };

public: // Operations
    /**
     * @brief Start a stroke with the tool.
     *
     * @param position The position on the canvas to start the stroke
     */
    virtual void start_stroke(glm::vec2 position);
    /**
     * @brief Continues a stroke with the tool.
     * Must be called between start_stroke and stop_stroke.
     *
     * @param Position on the canvas where the stroke will be continued to.
     */
    virtual void continue_stroke(glm::vec2 position);
    /**
     * @brief Stops the stroke.
     *
     */
    virtual void stop_stroke(glm::vec2 position);

protected: // Data
    bool in_stroke = false;
};

}
