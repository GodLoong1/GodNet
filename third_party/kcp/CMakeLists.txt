set(KCP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/ikcp.h
)

set(KCP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ikcp.c
)

add_library(kcp STATIC)

target_sources(
    kcp
    PRIVATE
        ${KCP_SOURCES}
    PUBLIC
        ${KCP_HEADERS}
)

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