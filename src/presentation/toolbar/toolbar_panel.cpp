#include "toolbar_panel.hpp"
#include "SDL3/SDL_opengl.h"
#include "imgui.h"
#include <stb_image.h>


namespace Presentation::Toolbar {

ToolbarPanel::ToolbarPanel(Application::Tools::Toolbar& toolbar) 
 : toolbar(toolbar) {
    load_icons();
}

void ToolbarPanel::render() {
    ImGui::Begin("Tools");


    for (const auto& tool : toolbar.get_tools()) {
        bool clicked = render_tool_button(tool);
        if (clicked)
            toolbar.set_active_tool(tool);
    }

    ImGui::End();
}

inline void ToolbarPanel::load_icons() {
    for (const auto& [type, path] : tool_icon_paths) {
        tool_icons.emplace(type, load_texture(path));
    }
}

inline uint32_t ToolbarPanel::load_texture(const std::string& path) {
    int image_width = 0, image_height = 0;
    unsigned char* image_data = stbi_load(path.c_str(), &image_width, &image_height, nullptr, 4);
    if (image_data == nullptr)
        return false;

    uint32_t image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    stbi_image_free(image_data);

    return image_texture;
}

inline bool ToolbarPanel::render_tool_button(const std::shared_ptr<Domain::Tools::Tool> tool) {
    // Determine if tool is selected
    const auto& active_tool = toolbar.get_active_tool();
    bool selected = tool == active_tool;
    // Get tool name as c string
    std::string tool_name = tool->get_tool_name();
    const char* tool_name_c = tool_name.c_str();
    ImVec2 text_size = ImGui::CalcTextSize(tool_name_c);
    ImVec2 total_size = {std::max(icon_size.x, text_size.x) + 4, icon_size.y + text_size.y + 4};
    ImVec2 pos = ImGui::GetCursorScreenPos();
    uint32_t texture_id;
    if (tool_icons.contains(tool->get_tool_type())) {
        texture_id = tool_icons[tool->get_tool_type()];
    } else {
        std::cerr << "Tool icon not found" << std::endl;
        return false;
    }

    // Render tool
    ImGui::BeginGroup();

    bool clicked = ImGui::InvisibleButton(tool_name_c, total_size);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float icon_x = pos.x + (total_size.x - icon_size.x) * 0.5f;
    draw_list->AddImage((ImTextureID)(intptr_t)texture_id, ImVec2(icon_x, pos.y), ImVec2(icon_x + icon_size.x, pos.y + icon_size.y));
    draw_list->AddText(ImVec2(pos.x + (total_size.x - text_size.x) * 0.5f, pos.y + icon_size.y + 4),
                    ImGui::GetColorU32(ImGuiCol_Text), tool_name_c);
    if (selected) {
        draw_list->AddRect(
            pos,
            ImVec2(pos.x + total_size.x, pos.y + total_size.y),
            ImGui::GetColorU32(ImGuiCol_ButtonActive), // or any custom color
            0.0f,   // rounding
            0,      // flags
            2.0f    // thickness
        );
    }

    ImGui::EndGroup();

    return clicked;
}

}
