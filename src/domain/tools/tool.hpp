#pragma once
#include "uuid_v4.h"
#include <string>


namespace Domain::Tools {

enum ToolType {
    TOOL_TYPE_PAN,
    TOOL_TYPE_BRUSH,
};

enum ToolUsage {
    TOOL_USAGE_CLICK,
    TOOL_USAGE_STROKE
};

class Tool {
public: // Ctor
    Tool() {
        // Generate UUID
        UUIDv4::UUIDGenerator<std::mt19937_64> uuid_generator;
        uuid = uuid_generator.getUUID();
    }
    ~Tool() = default;

public: // Accessors
    UUIDv4::UUID get_uuid() const { return uuid; }
    virtual std::string get_tool_name() const = 0;
    virtual ToolType get_tool_type() const = 0;
    virtual ToolUsage get_tool_usage() const = 0;

protected: // Data
    UUIDv4::UUID uuid;
};

}
