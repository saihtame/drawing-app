#include "editor_runtime.hpp"
#include <glad/gl.h>
#include <iostream>
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

EditorRuntime::EditorRuntime(std::shared_ptr<Application::Services::DocumentService> doc_service) 
    : doc_service(doc_service) {
}

int EditorRuntime::run() {
    initialize_window();

    return 0; // Indicate exit without error
}

inline void EditorRuntime::initialize_window() {

}

}

