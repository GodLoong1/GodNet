set(GODNET_HEADERS
    util/lock_free_queue.hpp
    util/noncopyable.hpp
    util/system.hpp
    util/logger.hpp
    util/endian.hpp

    network/endpoint.hpp
    network/event_channel.hpp
    network/event_loop.hpp
    network/event_loop_thread.hpp
    network/event_loop_thread_pool.hpp
    network/event_poller.hpp
    network/timer.hpp
    network/timer_queue.hpp
    network/socket.hpp
    network/tcp_acceptor.hpp
    network/tcp_socket.hpp
    network/message_buffer.hpp
)

set(GODNET_SOURCES
    util/system.cpp
    util/logger.cpp

    network/endpoint.cpp
    network/event_channel.cpp
    network/event_loop.cpp
    network/event_loop_thread.cpp
    network/event_loop_thread_pool.cpp
    network/event_poller.cpp
    network/timer.cpp
    network/timer_queue.cpp
    network/socket.cpp
    network/tcp_acceptor.cpp
    network/tcp_socket.cpp
    network/message_buffer.cpp
)

# 创建库
add_library(godnet STATIC ${GODNET_HEADERS} ${GODNET_SOURCES})
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