#!/bin/bash

# 项目名称
project_name="project-name"

# 创建目录
mkdir -p $project_name/src/module1
mkdir -p $project_name/src/module2
mkdir -p $project_name/include/module1
mkdir -p $project_name/include/module2
mkdir -p $project_name/lib
mkdir -p $project_name/bin
mkdir -p $project_name/test
mkdir -p $project_name/docs
mkdir -p $project_name/scripts

# 创建文件
touch $project_name/src/main.cpp
touch $project_name/.gitignore
echo "# $project_name" > $project_name/README.md
touch $project_name/CMakeLists.txt
