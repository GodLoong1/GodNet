file(GLOB_RECURSE SPDLOG_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/spdlog/*.h
)

file(GLOB_RECURSE SPDLOG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(spdlog STATIC ${SPDLOG_SOURCES} ${SPDLOG_HEADERS})
target_compile_definitions(
    spdlog
        PUBLIC
    SPDLOG_COMPILED_LIB
    SPDLOG_FMT_EXTERNAL
)

target_link_libraries(
    spdlog
    PRIVATE
        godnet-third-party-interface
    PUBLIC
        fmt
)

target_include_directories(
    spdlog
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(
    spdlog
    PROPERTIES
    FOLDER
    "third_party"
)