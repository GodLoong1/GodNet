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
        -w
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /w0
    >
)

# 设置编译选项
add_library(godnet-compile-option-interface INTERFACE)
if(GODNET_ASAN)
    target_compile_options(godnet-compile-option-interface INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang>:
            -fno-omit-frame-pointer
            -fsanitize=address
            -fsanitize-recover=address
            -fsanitize-address-use-after-scope
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            /fsanitize=address
        >
    )
    target_compile_definitions(godnet-compile-option-interface INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:
            _DISABLE_STRING_ANNOTATION
            _DISABLE_VECTOR_ANNOTATION
        >
    )
    target_link_options(godnet-compile-option-interface INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang>:
            -fno-omit-frame-pointer
            -fsanitize=address
            -fsanitize-recover=address
            -fsanitize-address-use-after-scope
        >
    )
    message(STATUS "** Enable Address Sanitizer ASan")
endif()

target_compile_options(godnet-compile-option-interface INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
        -fno-delete-null-pointer-checks
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /permissive-
        /Zc:throwingNew
        /utf-8
        /wd4351
        /wd4091
        /w15038
        /w34100
        /w34101
        /w34189
        /w34389
        /w35054
        /we4263
        /we4264
    >
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:
        -g3
    >
)

# 核心编译选项
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

message(STATUS "** Compiler: ${CMAKE_CXX_COMPILER_ID}")