set(FMT_HEADERS
    include/fmt/args.h
    include/fmt/base.h
    include/fmt/chrono.h
    include/fmt/color.h
    include/fmt/compile.h
    include/fmt/core.h
    include/fmt/format.h
    include/fmt/format-inl.h
    include/fmt/os.h
    include/fmt/ostream.h
    include/fmt/printf.h
    include/fmt/ranges.h
    include/fmt/std.h
    include/fmt/xchar.h
)

set(FMT_SOURCES
    src/format.cc
    src/os.cc
)

add_library(fmt STATIC ${FMT_HEADRES} ${FMT_SOURCES})

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

set_target_properties(
    fmt
    PROPERTIES
    FOLDER
    "third_party"
)