#include "ui.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <glad/gl.h>


namespace Presentation {

std::shared_ptr<UI> UI::create(Application::Documents::DocumentService& doc_service,
                               SDL_Window* window, void* gl_context) {
    auto ui = std::shared_ptr<UI>(new UI(doc_service, window, gl_context));
    doc_service.register_observer(ui);
    return ui;
}

UI::UI(Application::Documents::DocumentService& doc_service, SDL_Window* window, void* gl_context)
 : window(window), canvas_panel_service(doc_service), menu_bar(canvas_panel_service, doc_service, popup_manager), layers_panel(doc_service), doc_service(doc_service) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable docking
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    setup_style();

    // Setup scaling
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
 
    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    doc_service.deregister_observer(weak_from_this());
}

bool UI::process_event(SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    return (io.WantCaptureKeyboard || io.WantCaptureMouse);
}

void UI::render() {
    // Clear screen
    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Main Menu Bar
    menu_bar.render();

    // Render popups
    popup_manager.render();

    // Prepare dock space
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Replicate DockSpaceOverViewport but push a transparent ImGuiCol_WindowBg so the
    // dock node root fill does not paint over the glClear color in the canvas area.
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockspaceHost", nullptr, host_flags);
    ImGui::PopStyleVar(3);

    uint32_t dockspace_id = ImGui::GetID("DockSpace");

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
        ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::PopStyleColor();

    ImGui::End();

    // Arrange ui components on first render
    if (first_render) {
        first_render = false;
        reset_layout(dockspace_id, viewport);
    }

    // Render ui components
    ImGui::Begin("Tools");
    ImGui::Text("I start docked on the left.");
    ImGui::End();
    layers_panel.render();

    // Render canvas panels
    std::vector<UUIDv4::UUID> uuids_to_remove;
    for (const auto& doc_panel_pair : canvas_panel_service.get_panels()) {
        bool should_remove = doc_panel_pair.second->render();
        if (should_remove) {
            uuids_to_remove.push_back(doc_panel_pair.first);
        }
    }
    // Remove documents from DocumentService, if they have been closed
    for (const auto& uuid : uuids_to_remove) {
        doc_service.close_document(uuid);
    }

    // Render frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::document_service_update() {
    // Get all loaded documents for doc service
    const auto& loaded_docs = doc_service.get_documents();

    for (const auto& [uuid, document] : loaded_docs) {
        // Check that we have a canvas window for each loaded document
        if (!canvas_panel_service.contains(uuid)) {
            canvas_panel_service.add_panel(uuid, document);
            // Dock the new panel into the central canvas area
            if (auto panel = canvas_panel_service.get_canvas_panel(uuid).lock()) {
                ImGui::DockBuilderDockWindow(panel->get_panel_name().c_str(), canvas_dock_id);
            }
        }
    }

    // Check if we have canvas for non loaded document
    std::vector<UUIDv4::UUID> uuids_to_remove;
    for (const auto& doc_panel_pair : canvas_panel_service.get_panels()) {
        if (!loaded_docs.contains(doc_panel_pair.first)) {
            uuids_to_remove.push_back(doc_panel_pair.first);
        }
    }
    for (const auto& uuid : uuids_to_remove) {
        canvas_panel_service.remove_panel(uuid);
    }
}

inline void UI::reset_layout(uint32_t dockspace_id, ImGuiViewport* viewport) {
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

    // Split 30% of the dockspace into a node on each side.
    constexpr float side_ratio = 0.3f;
    ImGuiID remaining = dockspace_id;
    ImGuiID left_dock_id = ImGui::DockBuilderSplitNode(
        remaining,
        ImGuiDir_Left,
        side_ratio,
        nullptr,
        &remaining
    );
    ImGuiID right_dock_id = ImGui::DockBuilderSplitNode(
        remaining,
        ImGuiDir_Right,
        side_ratio / (1.0f - side_ratio),
        nullptr,
        &remaining
    );
    // 'remaining' is the central leaf node; canvas panels dock here.
    canvas_dock_id = remaining;

    // The window name must exactly match the name passed to ImGui::Begin().
    ImGui::DockBuilderDockWindow("Tools", left_dock_id);
    ImGui::DockBuilderDockWindow("Layers", right_dock_id);
    // Canvas panels
    for (const auto& doc_panel_pair : canvas_panel_service.get_panels()) {
        ImGui::DockBuilderDockWindow(doc_panel_pair.second->get_panel_name().c_str(), canvas_dock_id);
    }

    ImGui::DockBuilderFinish(dockspace_id);
}

/* Theme was found at: https://github.com/ocornut/imgui/issues/707#issuecomment-4107169777 */
inline void UI::setup_style() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- 1. Sizing and Spacing (Soft & Modern) ---
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;

    // --- 2. Borders & Rounding ---
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f; // Minimalist look
    style.PopupBorderSize = 1.0f;

    // --- 3. The Catppuccin Mocha Palette ---
    // Base: #1e1e2e | Mantle: #181825 | Crust: #11111b
    // Text: #cdd6f4 | Subtext0: #a6adc8 | Surface0: #313244
    // Lavender: #b4befe | Sapphire: #74c7ec | Mauve: #cba6f7

    // Text
    colors[ImGuiCol_Text] = ImVec4(0.80f, 0.84f, 0.96f, 1.00f); // Text
    colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.45f, 0.55f, 1.00f); // Surface1

    // Backgrounds
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base
    colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f); // Mantle
    colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.11f, 0.96f); // Crust

    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f); // Surface0
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frames (Inputs, etc.)
    colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f); // Surface0
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.35f, 1.00f); // Surface1
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f); // Surface2

    // Title Bars
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f); // Mantle
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f); // Crust

    // Menus
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);

    // Scrollbars
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f); // Surface2
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.37f, 0.38f, 0.51f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.42f, 0.45f, 0.55f, 1.00f);

    // Interactables
    colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f); // Lavender
    colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.78f, 0.93f, 1.00f); // Sapphire
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.78f, 0.93f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.65f, 0.97f, 1.00f); // Mauve
    colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.55f, 0.87f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.26f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);

    // Misc
    colors[ImGuiCol_PlotLines] = ImVec4(0.94f, 0.72f, 0.42f, 1.00f); // Marigold
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f); // Lavender

#ifdef IMGUI_HAS_DOCK
    colors[ImGuiCol_DockingPreview] = ImVec4(0.71f, 0.75f, 1.00f, 0.50f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
#endif

}

}

