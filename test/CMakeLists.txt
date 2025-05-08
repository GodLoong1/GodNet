add_executable(test test.cpp)
target_link_libraries(test fmt)

add_executable(endpoint_test endpoint_test.cpp)
target_link_libraries(endpoint_test godnet)
set_target_compile(endpoint_test)

add_executable(event_loop_test event_loop_test.cpp)
target_link_libraries(event_loop_test godnet)

add_executable(tcp_acceptor_test tcp_acceptor_test.cpp)
target_link_libraries(tcp_acceptor_test godnet)