#include "ui.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "presentation/editor_runtime.hpp"
#include <glad/gl.h>


namespace Presentation {

std::shared_ptr<UI> UI::create(Application::Documents::DocumentService& doc_service,
                               Application::Tools::Toolbar& toolbar,
                               SDL_Window* window, void* gl_context) {
    auto ui = std::shared_ptr<UI>(new UI(doc_service, toolbar, window, gl_context));
    doc_service.register_observer(ui);
    return ui;
}

UI::UI(Application::Documents::DocumentService& doc_service,
       Application::Tools::Toolbar& toolbar,
       SDL_Window* window, void* gl_context)
 : window(window), canvas_panel_service(doc_service),
   menu_bar(canvas_panel_service, doc_service, popup_manager),
   layers_panel(doc_service), doc_service(doc_service), toolbar(toolbar),
   toolbar_panel(toolbar), input_manager(doc_service, toolbar, canvas_panel_service) {
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

bool UI::process_event(SDL_Event& event) {
    // Always forward the event to ImGui first so its input state stays in sync,
    // even when the event ends up handled by the InputManager.
    // Note: the return value only tells whether the backend handled the event,
    // not whether ImGui wants it, so it cannot be used for routing.
    ImGui_ImplSDL3_ProcessEvent(&event);

    // A stroke in progress owns the mouse until the button is released, so it
    // keeps receiving motion/up events even when the cursor leaves the canvas.
    if (input_manager.is_dragging())
        return input_manager.process_event(event);

    const bool is_mouse_event = event.type == SDL_EVENT_MOUSE_MOTION ||
                                event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                                event.type == SDL_EVENT_MOUSE_BUTTON_UP ||
                                event.type == SDL_EVENT_MOUSE_WHEEL;
    const bool is_keyboard_event = event.type == SDL_EVENT_KEY_DOWN ||
                                   event.type == SDL_EVENT_KEY_UP ||
                                   event.type == SDL_EVENT_TEXT_INPUT ||
                                   event.type == SDL_EVENT_TEXT_EDITING;

    // ImGui gets first refusal on events aimed at its windows.
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if ((is_mouse_event && io.WantCaptureMouse) ||
        (is_keyboard_event && io.WantCaptureKeyboard))
        return true;

    return input_manager.process_event(event);
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
    toolbar_panel.render();
    layers_panel.render();
    canvas_panel_service.render();

    // Let mouse input fall through the dockspace host to the canvas below it.
    update_canvas_hit_hole(dockspace_id);

    // Render frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::document_service_update() {
    // Get all loaded documents for doc service
    const auto& loaded_docs = doc_service.get_documents();

    // Check that we have a canvas window for each loaded document
    for (const auto& [uuid, document] : loaded_docs) {
        if (!canvas_panel_service.contains(uuid)) {
            canvas_panel_service.add_panel(uuid, document);
            // Dock the new panel into the central canvas area
            if (auto panel = canvas_panel_service.get_canvas_panel(uuid).lock()) {
                ImGui::DockBuilderDockWindow(panel->get_panel_name().c_str(), canvas_dock_id);
            }
        }
    }

    // Check if we have canvas for non loaded document that needs to be removed
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

inline void UI::update_canvas_hit_hole(uint32_t dockspace_id) {
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
    if (node == nullptr)
        return;

    // DockSpace() submits a child window ("<host>/DockSpace_XXXXXXXX") that
    // hosts the dock node. That child is what actually gets hovered behind the
    // canvas, and its parent spans the same area, so both need the hole.
    ImGuiWindow* node_host = node->HostWindow;
    if (node_host == nullptr)
        return;

    // Do not steal hover while a window is being dragged, otherwise dropping a
    // panel back onto the canvas would stop working.
    const ImGuiPayload* payload = ImGui::GetDragDropPayload();
    if (payload != nullptr && payload->IsDataType(IMGUI_PAYLOAD_TYPE_WINDOW))
        return;

    auto visible_panel = canvas_panel_service.get_visible_panel().lock();
    if (visible_panel == nullptr || !visible_panel->is_visible())
        return;

    const ImVec2 pos = visible_panel->get_pos();
    const ImVec2 size = visible_panel->get_size();
    if (size.x <= 0.0f || size.y <= 0.0f)
        return;

    // Expand sideways and down over the window padding, but not up into the
    // tab bar so tabs remain clickable.
    const ImVec2 padding = ImGui::GetStyle().WindowPadding;
    const ImVec2 hole_pos(pos.x - padding.x, pos.y);
    const ImVec2 hole_size(size.x + padding.x * 2.0f, size.y + padding.y);

    // ImGui only supports one hole per window, and PassthruCentralNode already
    // registers one while the central node is empty. Only take it over when the
    // mouse is actually over the canvas, e.g. after the canvas was moved out of
    // the central node.
    if (node_host->HitTestHoleSize.x != 0) {
        const ImVec2 mouse = ImGui::GetIO().MousePos;
        const bool mouse_over_canvas =
            mouse.x >= hole_pos.x && mouse.x <= hole_pos.x + hole_size.x &&
            mouse.y >= hole_pos.y && mouse.y <= hole_pos.y + hole_size.y;
        if (!mouse_over_canvas)
            return;
    }

    ImGuiWindow* targets[] = { node_host, node_host->ParentWindow };
    for (ImGuiWindow* window : targets) {
        if (window == nullptr)
            continue;
        if (window->HitTestHoleSize.x != 0)
            window->HitTestHoleSize = ImVec2ih(0, 0);
        ImGui::SetWindowHitTestHole(window, hole_pos, hole_size);
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

