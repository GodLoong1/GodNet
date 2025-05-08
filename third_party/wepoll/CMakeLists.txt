set(WEPOLL_HEADERS
    wepoll.h
)

set(WEPOLL_SOURCES
    wepoll.c
)

if(MSVC)
    add_library(wepoll STATIC ${WEPOLL_HEADERS} ${WEPOLL_SOURCES})

    target_link_libraries(
        wepoll
        PRIVATE
        godnet-third-party-interface
    )

    target_include_directories(
        wepoll
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    add_library(wepoll INTERFACE)
endif()

set_target_properties(
    wepoll
    PROPERTIES
    FOLDER
    "third_party"
)