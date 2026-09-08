#pragma once
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "application/documents/document_service.hpp"
#include "menu_bar.hpp"
#include "popups/popup_manager.hpp"
#include "presentation/canvas_panel.hpp"


// ImGui forward declarations
class ImGuiViewport;

namespace Presentation {

class UI final : public std::enable_shared_from_this<UI>, public Application::Documents::DocumentServiceObserver {
public:
    // Replacement for constructor
    // Ensures that UI is created inside a shared_ptr, which it requires
    // to create weak_ptrs from itself.
    static std::shared_ptr<UI> create(Application::Documents::DocumentService& doc_service,
       SDL_Window* window, void* gl_context);
    ~UI();

private:
    UI(Application::Documents::DocumentService& doc_service,
       SDL_Window* window, void* gl_context);

public:
    bool processEvent(SDL_Event* event);
    void render();

public:
    virtual void document_service_update();

private: // Dependencies
    Application::Documents::DocumentService& doc_service;

private: // Members
    bool first_render = true;
    SDL_Window* window;
    // UI Components:
    Popups::PopupManager popup_manager;
    MenuBar menu_bar;
    std::vector<std::unique_ptr<CanvasPanel>> canvas_panels;

private: // Render methods
    inline void reset_layout(uint32_t dockspace_id, ImGuiViewport* viewport);

private: // Setup methods
    inline void setup_style();

private: // Constants
    static constexpr const char* glsl_version = "#version 460";
};

}
