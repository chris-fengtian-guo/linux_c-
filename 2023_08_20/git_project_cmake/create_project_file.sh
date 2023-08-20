# 创建项目根目录
mkdir -p project_root

# 进入项目根目录
cd project_root

# 创建根目录的CMakeLists.txt文件
touch CMakeLists.txt

# 为每个模块创建目录、子目录和文件
for module in data_manage msg_process behavior_tree; do
    mkdir -p $module/src
    mkdir -p $module/include
    mkdir -p $module/test

    touch $module/CMakeLists.txt
    touch $module/src/$module.cpp
    touch $module/include/$module.h

    # 为每个人创建测试主程序文件
    for person in Alice Bob Charlie David Eve; do
        # 将名称转换为小写
        person_lc=$(echo $person | tr 'A-Z' 'a-z')
        touch $module/${person_lc}_test_main.cpp
    done
done

# 创建主程序文件
touch main.cpp
