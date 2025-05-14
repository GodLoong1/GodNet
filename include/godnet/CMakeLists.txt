set(GODNET_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/util/lock_free_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/noncopyable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/system.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/endian.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/network/endpoint.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_channel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_loop.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_loop_thread.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_loop_thread_pool.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_poller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/timer_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/socket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/tcp_acceptor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/tcp_socket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/message_buffer.hpp
)

set(GODNET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/util/system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/logger.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/network/endpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_channel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_loop_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_loop_thread_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/event_poller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/timer_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/socket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/tcp_acceptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/tcp_socket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/network/message_buffer.cpp
)

add_library(godnet STATIC)

target_sources(
    godnet
    PRIVATE
        ${GODNET_SOURCES}
    PUBLIC
        ${GODNET_HEADERS}
)

target_link_libraries(
    godnet
    PRIVATE
        godnet-core-interface
    PUBLIC
        fmt
        spdlog
        nlohmann-json
        kcp
        wepoll
        ws2_32
)

target_include_directories(
    godnet
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)