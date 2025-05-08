add_executable(test test.cpp)
target_link_libraries(test PRIVATE godnet-core-interface PUBLIC godnet)

add_executable(endpoint_test endpoint_test.cpp)
target_link_libraries(endpoint_test PRIVATE godnet-core-interface PUBLIC godnet)

add_executable(event_loop_test event_loop_test.cpp)
target_link_libraries(event_loop_test PRIVATE godnet-core-interface PUBLIC godnet)

add_executable(tcp_acceptor_test tcp_acceptor_test.cpp)
target_link_libraries(tcp_acceptor_test PRIVATE godnet-core-interface PUBLIC godnet)