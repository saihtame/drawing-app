add_library(${DOMAIN_TARGET} STATIC
    document.cpp
    layer.cpp
)

target_include_directories(${DOMAIN_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)
