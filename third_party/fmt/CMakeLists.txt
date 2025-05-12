file(GLOB_RECURSE FMT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/fmt/*.h
)

file(GLOB_RECURSE FMT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

add_library(fmt STATIC)

target_sources(
    fmt
    PRIVATE
        ${FMT_SOURCES}
    PUBLIC
        ${FMT_HEADERS}
)

target_link_libraries(
    fmt
    PRIVATE
        godnet-third-party-interface
)

target_include_directories(
    fmt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)