set(IMGUI_SRC_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

add_library(imgui STATIC
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_tables.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${IMGUI_SRC_DIR}/imgui_demo.cpp
    ${IMGUI_SRC_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_SRC_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_SRC_DIR}
    ${IMGUI_SRC_DIR}/backends
)

target_link_libraries(imgui PUBLIC
    SDL3::SDL3
    OpenGL::GL
)

target_compile_features(imgui PUBLIC cxx_std_17)