cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 添加模块子目录
add_subdirectory(data_manage)
add_subdirectory(msg_process)
add_subdirectory(behavior_tree)

# 创建主程序可执行文件
add_executable(main main.cpp)

# 将主程序链接到三个模块
target_link_libraries(main data_manage msg_process behavior_tree)
