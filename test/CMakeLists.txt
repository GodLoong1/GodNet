add_executable(test test.cpp)
target_link_libraries(test fmt::fmt)

add_executable(inet_address_test inet_address_test.cpp)
target_link_libraries(inet_address_test godnet)

add_executable(event_loop_test event_loop_test.cpp)
target_link_libraries(event_loop_test godnet)