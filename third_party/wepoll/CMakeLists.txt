if(MSVC)
    set(WEPOLL_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/wepoll.h
    )

    set(WEPOLL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/wepoll.c
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
else()
    add_library(wepoll INTERFACE)
endif()