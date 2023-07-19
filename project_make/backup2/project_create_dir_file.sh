#!/bin/bash

# 创建目录结构
mkdir -p src/network
mkdir -p src/services
mkdir -p src/exception
mkdir -p include/network
mkdir -p include/services
mkdir -p include/exception

# 创建 .cpp 文件
touch src/main.cpp
touch src/network/UnixSocketServer.cpp
touch src/network/UnixSocketClient.cpp
touch src/network/UdpSocketServer.cpp
touch src/network/UdpSocketClient.cpp
touch src/services/MessageService.cpp
touch src/services/ProcessManager.cpp
touch src/exception/ExceptionHandler.cpp

# 创建 .h 文件
touch include/network/UnixSocketServer.h
touch include/network/UnixSocketClient.h
touch include/network/UdpSocketServer.h
touch include/network/UdpSocketClient.h
touch include/services/MessageService.h
touch include/services/ProcessManager.h
touch include/exception/ExceptionHandler.h
