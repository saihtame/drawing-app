add_library(${PRESENTATION_TARGET} STATIC
    editor_runtime.cpp
    ui.cpp
    menu_bar.cpp
    layers_panel.cpp
    canvas_panels/canvas_panel.cpp
    canvas_panels/canvas_panel_service.cpp
    popups/popup_manager.cpp
    popups/new_file_popup.cpp
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

