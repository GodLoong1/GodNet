add_executable(test test.cpp)
target_link_libraries(test fmt::fmt)

add_executable(inet_address_test inet_address_test.cpp)
target_link_libraries(inet_address_test godnet)
# set_target_compile(inet_address_test)