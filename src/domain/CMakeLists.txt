set(DOMAIN_TARGET drawing_app_domain)

add_library(${DOMAIN_TARGET} STATIC
    document.cpp
    layer.cpp
)
