@echo off
set BUILD_TYPE=Debug
set BUILD_DIR=build

:: 配置
cmake -S . -B %BUILD_DIR% ^
  -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE ^
  -G Ninja

:: 构建
ninja -C %BUILD_DIR% -j %JOBS%
