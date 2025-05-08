# 设置Cmake最低版本
cmake_minimum_required(VERSION 3.5)
project(GodNet
    VERSION 1.0
    LANGUAGES CXX C
    DESCRIPTION "GodNet network library"
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# 设置输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# 配置编译类型（Debug/Release）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# 设置编译选项
function(set_target_compile target)
    target_compile_definitions(${target} PRIVATE 
        $<$<CONFIG:Release>:
            NDEBUG
        >
    )
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} PRIVATE 
            /W4     # 高警告级别
            /WX     # 警告视作错误
            /permissive-  # 符合标准模式
            /utf-8  # 强制使用UTF-8编码
            $<$<CONFIG:Debug>:
                /Od
                /Zi
            >
            $<$<CONFIG:Release>:
                /O2     # 最大优化
                /fp:fast
                /GL     # 全程序优化
            >
        )
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall
            -rdynamic
            -Wextra
            -Wpedantic
            -Werror
            -Wshadow             # 变量隐藏警告
            -Wunused             # 未使用警告
            -Wdouble-promotion   # float到double提升警告
            -Wformat=2           # 格式化字符串检查
            $<$<CONFIG:Debug>:
                -O0
                -g
            >
            $<$<CONFIG:Release>:
                -O3
                -march=native
                -ffunction-sections
                -fdata-sections
            >
        )
    endif()
endfunction()
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

# 头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/fmt/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/wepoll)

# 添加子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/include/godnet)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party)