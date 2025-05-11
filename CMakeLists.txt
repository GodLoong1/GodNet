# 设置Cmake最低版本
cmake_minimum_required(VERSION 3.5)
project(GodNet
    VERSION 1.0
    LANGUAGES CXX C
    DESCRIPTION "GodNet network library"
)

option(GODNET_ASAN "Enable Address Sanitizer" ON)

# 配置编译类型（Debug/Release）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
endif()
message(STATUS "** Build type: ${CMAKE_BUILD_TYPE}")

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 
message(STATUS "** C++ standard: ${CMAKE_CXX_STANDARD}")

# 设置输出目录
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_TOLOWER}"
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_TOLOWER}"
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_TOLOWER}"
)

# 添加子配置
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(compile_options)
include(find_ws2_32)

# 添加子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/include/godnet)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party)