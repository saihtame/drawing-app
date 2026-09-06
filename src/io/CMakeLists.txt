add_library(${IO_TARGET} STATIC
    documents/file_document_service.cpp
)

target_include_directories(${IO_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${IO_TARGET}
    PUBLIC
        ${APPLICATION_TARGET}
        drawing_app_config
)
