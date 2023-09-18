cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 检查处理器架构
message("SYSTEM INFO ${CMAKE_SYSTEM_PROCESSOR}")
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64|amd64")
  message(STATUS "Configuring build for x86_64")
  # 设置用于 x86_64 的编译和链接选项
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
  # 其他 x86_64 相关的设置
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|ARM")
  message(STATUS "Configuring build for ARM")
  # 设置用于 ARM 的编译和链接选项
  # 其他 ARM 相关的设置
else()
  message(STATUS "Configuring build for unknown architecture")
  # 设置用于未知架构的编译和链接选项
endif()
