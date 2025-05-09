add_library(nlohmann-json INTERFACE)

target_include_directories(
    nlohmann-json
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(
    nlohmann-json
    PROPERTIES
    FOLDER
    "third_party"
)