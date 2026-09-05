add_library(${PRESENTATION_TARGET} STATIC
    editor_runtime.cpp
    ui.cpp
)

target_include_directories(${PRESENTATION_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PRESENTATION_TARGET}
    PRIVATE
        SDL3::SDL3
        glad
        stb::image
        imgui
    PUBLIC
        ${APPLICATION_TARGET}
        drawing_app_config
)

