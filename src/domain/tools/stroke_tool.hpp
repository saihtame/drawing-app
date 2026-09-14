#pragma once
#include "domain/tools/tool.hpp"
#include <glm/ext/vector_float2.hpp>


namespace Domain::Tools {

class StrokeTool : public Tool {
public: // Operations
    virtual void start_stroke(glm::vec2 position);
    virtual void continue_stroke(glm::vec2 position);
    virtual void stop_stroke(glm::vec2 position);

protected: // Data
    bool in_stroke = false;
};

}
