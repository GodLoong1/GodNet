set(KCP_HEADERS
    ikcp.h
)

set(KCP_SOURCES
    ikcp.c
)

add_library(kcp STATIC ${KCP_HEADERS} ${KCP_SOURCES})

target_link_libraries(
    kcp
    PRIVATE
        godnet-third-party-interface
)

target_include_directories(
    kcp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
    kcp
    PROPERTIES
    FOLDER
    "third_party"
)