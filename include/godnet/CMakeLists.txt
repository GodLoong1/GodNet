set(GODNET_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/util/lock_free_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/noncopyable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/system.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/endian.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/net/inet_address.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_channel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_loop.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_loop_thread.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_loop_thread_pool.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_poller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/timer_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/socket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/tcp_acceptor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/tcp_socket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/message_buffer.hpp
)

set(GODNET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/util/system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/logger.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/net/inet_address.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_channel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_loop_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_loop_thread_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/event_poller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/timer_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/socket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/tcp_acceptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/tcp_socket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net/message_buffer.cpp
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