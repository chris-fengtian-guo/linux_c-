// src/command-handler/MessageHandler.cpp
#include "MessageHandler.h"
#include <iostream>

MessageHandler::MessageHandler() {}

MessageHandler::~MessageHandler() {}

void MessageHandler::handleCommand(const Command& command) {
    // 原先的handleCommand代码
}

bp::child MessageHandler::runBinaryWithArgument2(const char* binaryPath, const char* argument) {
    // 在子进程中执行二进制程序
    bp::child c(bp::exe = binaryPath, bp::args = {argument});

    std::cout << "run process pid=" << c.id() << "\n, running=" << c.running() << "\n";
    c.wait();
    // 获取进程的退出状态
    if (!c.running()) {
        std::cout << "The process has exited with status: " << c.exit_code() << std::endl;
    }

    return c;
}
void MessageHandler::handleCommand(const Command& command) {
    // 处理command对象...
    auto child = this->runBinaryWithArgument2(binaryPath.c_str(), argument.c_str());
}
