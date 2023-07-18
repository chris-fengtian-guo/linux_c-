#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// 定义命令信息数据结构
struct Command {
    std::string action; // 动作
    unsigned int behaviorTreeID; // 行为树ID
};

#endif // COMMAND_H
