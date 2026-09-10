add_library(${INFRASTRUCTURE_TARGET} STATIC
    io/document_file_serializer.cpp
    canvas/gpu_canvas.cpp
    canvas/gpu_canvas_layer.cpp
    canvas/gpu_canvas_factory.cpp
    rendering/shader.cpp
    rendering/shader_program.cpp
    rendering/plane.cpp
    rendering/material.cpp
    rendering/texture.cpp
    compute/texture_compositor.cpp
)

target_include_directories(${INFRASTRUCTURE_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${INFRASTRUCTURE_TARGET}
    PRIVATE
        glad
    PUBLIC
        ${APPLICATION_TARGET}
        drawing_app_config
)
