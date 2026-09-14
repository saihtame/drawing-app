#pragma once
#include <string>


namespace Domain::Tools {

enum ToolType {
    TOOL_TYPE_BRUSH,
};

class Tool {
public: // Accessors
    virtual std::string get_tool_name() const = 0;
    virtual ToolType get_tool_type() const = 0;
};

}
