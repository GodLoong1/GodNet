# 编译文件
set(GODNET_SRC
    network/inet_address.cpp
)

# 创建库
add_library(godnet ${GODNET_SRC})
set_target_compile(godnet)
if (BUILD_SHARED_LIBS)
    target_compile_definitions(godnet PRIVATE
        GODNET_DYNAMIC
    )
endif()