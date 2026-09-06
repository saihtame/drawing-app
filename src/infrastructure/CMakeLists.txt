add_library(${INFRASTRUCTURE_TARGET} STATIC
    io/document_file_serializer.cpp
    canvas/gpu_document_canvas.cpp
    canvas/gpu_document_canvas_factory.cpp
)

target_include_directories(${INFRASTRUCTURE_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${INFRASTRUCTURE_TARGET}
    PUBLIC
        ${APPLICATION_TARGET}
        drawing_app_config
)
