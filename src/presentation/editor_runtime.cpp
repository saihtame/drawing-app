#include "editor_runtime.hpp"
#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <iostream>
#include <memory>
#include <ostream>


namespace Presentation {

// OpenGL error callback function
void GLAPIENTRY MessageCallback(GLenum,
                 GLenum type,
                 GLuint,
                 GLenum severity,
                 GLsizei,
                 const GLchar* message,
                 const void*) {
    // Skip notifications
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    // Print error
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

EditorRuntime::EditorRuntime(Application::Documents::DocumentService& doc_service) 
    : doc_service(doc_service) {
}

EditorRuntime::~EditorRuntime() {
    if (gl_context != nullptr)
        SDL_GL_DestroyContext(gl_context);
    if (win != nullptr)
        SDL_DestroyWindow(win);
    SDL_Quit();
}

int EditorRuntime::run() {
    /* Initialization */
    if (initialize_window()) {
        std::cerr << "Failed to initialize windows" << std::endl;
        return -1;
    }
    ui = std::make_unique<UI>(doc_service, win, gl_context);

    /* Loop */
    while (!quit) {
        handle_input();
        render();
    }

    /* Exit */
    return 0; // Indicate exit without error
}

inline int EditorRuntime::initialize_window() {
    SDL_Init(SDL_INIT_VIDEO);

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0); // 2D app, no need for depth buffer

    // Create window
    win = SDL_CreateWindow("Drawing app", window_width, window_height, window_flags);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create OpenGL context
    gl_context = SDL_GL_CreateContext(win);
    if (gl_context == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Initialize glad with the OpenGL Context
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

#ifdef _DEBUG
    // Enable OpenGL debug messages to the callback
    std::cout << "Running in debug mode" << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif

    // Enable face culling
    glEnable(GL_CULL_FACE);
    return 0;
}

inline void EditorRuntime::handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Quit event
        if (event.type == SDL_EVENT_QUIT) {
            quit = true;
            continue;
        }
        // Window resize
        if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            window_resized(event.window.data1, event.window.data2);
            continue;
        }
        // Allow UI to capture events
        if (ui->processEvent(&event))
            continue;
    }
}

inline void EditorRuntime::render() {
    // Clear screen
    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render canvas

    // Render UI
    ui->render();

    // Swap frame buffers
    SDL_GL_SwapWindow(win);
}

inline void EditorRuntime::window_resized(float width, float height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}

}

