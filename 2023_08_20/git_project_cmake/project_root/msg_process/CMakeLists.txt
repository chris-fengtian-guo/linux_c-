# 添加库目标
add_library(msg_process src/msg_process.cpp)
target_include_directories(msg_process PUBLIC include)

# 为每个开发人员创建测试主程序
set(DEV_NAMES Alice Bob Charlie David Eve)

foreach(DEV_NAME ${DEV_NAMES})
    # 将开发人员名称转换为小写
    string(TOLOWER ${DEV_NAME} DEV_NAME_LC)

    # 创建测试主程序
    add_executable(${DEV_NAME_LC}_msg_process_test_main ${DEV_NAME_LC}_test_main.cpp)
    target_link_libraries(${DEV_NAME_LC}_msg_process_test_main msg_process)
endforeach()
