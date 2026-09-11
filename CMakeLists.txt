cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME drawing_app)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build a static version of the library" FORCE)
add_subdirectory(external/sdl3 EXCLUDE_FROM_ALL)

add_subdirectory(external/glad)
add_subdirectory(external/glm)
add_subdirectory(external/stb)
add_subdirectory(external/uuid_v4 EXCLUDE_FROM_ALL)
include(cmake/imgui.cmake)
add_subdirectory(src)

