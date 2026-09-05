#pragma once
#include "application/services/document_service.hpp"
#include <SDL3/SDL_video.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/vec4.hpp>

namespace Presentation {

class EditorRuntime {
public:
    EditorRuntime(std::shared_ptr<Application::Services::DocumentService> doc_service);
    ~EditorRuntime();

private: // Dependencies
    std::shared_ptr<Application::Services::DocumentService> doc_service;

public: // Public methods
    int run();

private: // Helper methods
    inline int initialize_window();
    inline void handle_input();
    inline void render();
    inline void window_resized(float width, float height);

private: // Constants
    const int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    const glm::vec4 background_color {1.0f, 1.0f, 1.0f, 1.0f};

private: // Members
    SDL_Window* win = nullptr;
    SDL_GLContext gl_context = nullptr;
    int window_width = 640;
    int window_height = 480;
    bool quit = false;
};

}
