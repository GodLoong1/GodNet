set(GODNET_HEADERS
    util/debug.hpp
    util/lock_free_queue.hpp
    util/noncopyable.hpp
    util/system.hpp

    network/endpoint.hpp
    network/event_channel.hpp
    network/event_loop.hpp
    network/event_poller.hpp
    network/timer.hpp
    network/timer_queue.hpp
    network/socket.hpp
    network/tcp_acceptor.hpp
    network/tcp_socket.hpp
)

set(GODNET_SOURCES
    util/debug.cpp
    util/system.cpp

    network/endpoint.cpp
    network/event_channel.cpp
    network/event_loop.cpp
    network/event_poller.cpp
    network/timer.cpp
    network/timer_queue.cpp
    network/socket.cpp
    network/tcp_acceptor.cpp
    network/tcp_socket.cpp
)

# 创建库
add_library(godnet STATIC ${GODNET_HEADERS} ${GODNET_SOURCES})
target_link_libraries(
    godnet
    PRIVATE
        godnet-core-interface
    PUBLIC
        fmt
)

target_include_directories(
    godnet
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)