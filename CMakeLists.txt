# 设置Cmake最低版本
cmake_minimum_required(VERSION 3.5)
project(GodNet
    VERSION 1.0
    LANGUAGES CXX C
    DESCRIPTION "GodNet network library"
)

# 配置编译类型（Debug/Release）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# 设置C++标准
set(CMAKE_CXX_EXTENSIONS OFF) 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# 设置输出目录
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_TOLOWER}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_TOLOWER}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE_TOLOWER}")

# 设置编译警告选项
add_library(godnet-warning-interface INTERFACE)
target_compile_options(godnet-warning-interface INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:
        -W
        -Wall
        -Wextra
        -Winit-self
        -Winvalid-pch
        -Wfatal-errors
        -Woverloaded-virtual
        -Wno-missing-field-initializers
        -Wno-maybe-uninitialized
    >
    $<$<CXX_COMPILER_ID:Clang>:
        -W
        -Wall
        -Wextra
        -Wimplicit-fallthrough
        -Winit-self
        -Wfatal-errors
        -Wno-mismatched-tags
        -Woverloaded-virtual
        -Wno-missing-field-initializers
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W3
    >
)

# 设置编译不警告选项
add_library(godnet-no-warning-interface INTERFACE)
target_compile_options(godnet-no-warning-interface INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
        -W
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W0
    >
)

# 设置编译选项
add_library(godnet-compile-option-interface INTERFACE)
target_compile_options(godnet-compile-option-interface INTERFACE
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:
        -g3
        -O0
        -fno-omit-frame-pointer
        -fsanitize=address
        -fsanitize-recover=address
        -fsanitize-address-use-after-scope
    >
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Release>>:
        -O2
        -DNDEBUG
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:
        /Zi
        /Od
        _DISABLE_STRING_ANNOTATION
        _DISABLE_VECTOR_ANNOTATION
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:
        /O2
        /DNDEBUG
    >
)
target_link_options(godnet-compile-option-interface INTERFACE
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:
        -fno-omit-frame-pointer
        -fsanitize=address
        -fsanitize-recover=address
        -fsanitize-address-use-after-scope
    >
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:
        /fsanitize=address
    > 
)

# 默认编译选项
add_library(godnet-core-interface INTERFACE)
target_link_libraries(godnet-core-interface INTERFACE
    godnet-compile-option-interface
    godnet-warning-interface
)

# 依赖库编译选项
add_library(godnet-third-party-interface INTERFACE)
target_link_libraries(godnet-third-party-interface INTERFACE
    godnet-compile-option-interface
    godnet-no-warning-interface
)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

# 添加子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/include/godnet)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party)