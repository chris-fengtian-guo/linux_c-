#!/bin/bash

# 创建目录结构
mkdir -p src/network
mkdir -p src/services
mkdir -p src/exception
mkdir -p src/command-handler
mkdir -p src/data-manager
mkdir -p src/behavior-trees/tree1
mkdir -p src/behavior-trees/tree2

mkdir -p include/network
mkdir -p include/services
mkdir -p include/exception
mkdir -p include/command-handler
mkdir -p include/data-manager
mkdir -p include/behavior-trees/tree1
mkdir -p include/behavior-trees/tree2

# 创建 .cpp 文件
touch src/main.cpp
touch src/network/UnixSocketServer.cpp
touch src/network/UnixSocketClient.cpp
touch src/network/UdpSocketServer.cpp
touch src/network/UdpSocketClient.cpp
touch src/services/MessageService.cpp
touch src/services/ProcessManager.cpp
touch src/exception/ExceptionHandler.cpp
touch src/command-handler/CommandHandler.cpp
touch src/data-manager/DataManager.cpp
touch src/behavior-trees/tree1/Tree1.cpp
touch src/behavior-trees/tree2/Tree2.cpp

# 创建 .h 文件
touch include/network/UnixSocketServer.h
touch include/network/UnixSocketClient.h
touch include/network/UdpSocketServer.h
touch include/network/UdpSocketClient.h
touch include/services/MessageService.h
touch include/services/ProcessManager.h
touch include/exception/ExceptionHandler.h
touch include/command-handler/CommandHandler.h
touch include/data-manager/DataManager.h
touch include/behavior-trees/tree1/Tree1.h
touch include/behavior-trees/tree2/Tree2.h
