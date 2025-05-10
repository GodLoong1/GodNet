#!/bin/bash

# 构建类型（可修改为 Release）
BUILD_TYPE=Debug

# 构建目录
BUILD_DIR=build

# 使用的并发线程数（默认等于 CPU 核心数）
JOBS=$(nproc)

# CMake 配置
cmake -S . -B $BUILD_DIR \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
  -G Ninja

# 多线程构建
ninja -C $BUILD_DIR -j $JOBS
