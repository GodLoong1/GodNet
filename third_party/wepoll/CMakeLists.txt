if(MSVC)
    set(WEPOLL_HEADERS
        wepoll.h
    )

    set(WEPOLL_SOURCES
        wepoll.c
    )

    add_library(wepoll STATIC)

    target_sources(
        wepoll
        PRIVATE
            ${WEPOLL_SOURCES}
        PUBLIC
            ${WEPOLL_HEADERS}
    )

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

    set_target_properties(
        wepoll
        PROPERTIES
        FOLDER
        "third_party"
    )
else()
    add_library(wepoll INTERFACE)
endif()