#pragma once
#include "application/tools/toolbar.hpp"
#include "domain/tools/brush_tool.hpp"
#include "domain/tools/tool.hpp"
#include "imgui.h"
#include <unordered_map>


namespace Presentation::Toolbar {

class ToolbarPanel final {
public: // Ctors
    ToolbarPanel(Application::Tools::Toolbar& toolbar);
    ~ToolbarPanel() = default;

public: // Public methods
    void render();

private: // Helpers
    inline void load_icons();
    inline uint32_t load_texture(const std::string& path);
    inline ImVec2 render_tool_button(const std::shared_ptr<Domain::Tools::Tool> tool);
    inline void render_active_tool_options();

private: // Data
    std::unordered_map<Domain::Tools::ToolType, int> tool_icons;

private: // Constants
    ImVec2 icon_size = {64, 64};
    std::unordered_map<Domain::Tools::ToolType, std::string> tool_icon_paths = {
        {Domain::Tools::TOOL_TYPE_PAN, "assets/icons/brush.png"},
        {Domain::Tools::TOOL_TYPE_BRUSH, "assets/icons/brush.png"},
    };

private: // Dependencies
    Application::Tools::Toolbar& toolbar;
};

}
