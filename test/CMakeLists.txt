add_executable(test test.cpp)
set_target_compile(test)

add_executable(inet_address_test inet_address_test.cpp)
target_link_libraries(inet_address_test godnet)