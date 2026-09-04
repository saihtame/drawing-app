add_library(${APPLICATION_TARGET} STATIC
    services/document_service.cpp
)

target_include_directories(${APPLICATION_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${APPLICATION_TARGET}
    PUBLIC
        ${DOMAIN_TARGET}
)
