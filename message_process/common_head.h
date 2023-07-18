#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// 定义命令信息数据结构
struct Command {
    unsigned int action; // 动作
    unsigned int behaviorTreeID; // 行为树ID
};
#define UNIX_SRV_PATH "/tmp/my_socket"

#endif // COMMAND_H
