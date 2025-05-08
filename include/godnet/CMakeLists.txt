# 编译文件
set(GODNET_SRC
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
add_library(godnet STATIC ${GODNET_SRC})
set_target_compile(godnet)
target_link_libraries(godnet fmt dl)