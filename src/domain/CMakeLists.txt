add_library(${DOMAIN_TARGET} STATIC
    documents/document.cpp
    documents/canvas.cpp
)

target_include_directories(${DOMAIN_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${DOMAIN_TARGET}
    PUBLIC
        drawing_app_config
)
