#pragma once
#include "uuid_v4.h"
#include <string>


namespace Domain::Tools {

enum ToolType {
    TOOL_TYPE_PAN,
    TOOL_TYPE_BRUSH,
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
    /**
     * @brief Get the display name of the tool
     */
    virtual std::string get_tool_name() const = 0;
    /**
     * @brief Get the type of the tool
     */
    virtual ToolType get_tool_type() const = 0;

protected: // Data
    UUIDv4::UUID uuid;
};

}
