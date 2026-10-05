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
     * @param Start position in canvas space
     */
    virtual void start_stroke(glm::vec2 position);
    /**
     * @brief Continues a stroke with the tool.
     * Must be called between start_stroke and stop_stroke.
     *
     * @param Position to continue the drag to in canvas space
     */
    virtual void continue_stroke(glm::vec2 position);
    /**
     * @brief Stops the stroke.
     *
     * @param Last position of the drag in canvas space
     */
    virtual void stop_stroke(glm::vec2 position);

protected: // Data
    bool in_stroke = false;
};

}
