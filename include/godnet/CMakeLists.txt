# 编译文件
set(GODNET_SRC
    exception.cpp
    network/inet_address.cpp
)

# 创建库
add_library(godnet ${GODNET_SRC})
set_target_compile(godnet)
set_target_properties(godnet PROPERTIES DEFINE_SYMBOL "")

# 设置导出宏
target_compile_definitions(godnet PRIVATE
    $<$<BOOL:${BUILD_SHARED_LIBS}>:
        GODNET_DYNAMIC
    >
)