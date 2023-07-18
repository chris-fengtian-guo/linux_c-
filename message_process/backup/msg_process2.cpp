#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector> // 添加vector头文件

#include "common_head.h"

// 封装的函数，启动一个二进制程序并传递一个参数
int runBinaryWithArgument(const char* binaryPath, const char* name, const char* argument) {
    pid_t pid = fork();
    if (pid == 0) {
        // 在子进程中执行二进制程序
        execlp(binaryPath, name, argument, nullptr);
        exit(0);
    } else if (pid > 0) {
        // 在父进程中等待子进程结束
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "Binary " << binaryPath << " exited with status: " << exitStatus << std::endl;
            return exitStatus;
        } else if (WIFSIGNALED(status)) {
            int signalCode = WTERMSIG(status);
            std::cout << "Binary " << binaryPath << " terminated by signal: " << signalCode << std::endl;
            return -signalCode;
        } else {
            std::cout << "Binary " << binaryPath << " terminated abnormally" << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Failed to fork a new process" << std::endl;
        return -1;
    }
}

// 行为树路径数组
const std::vector<std::string> behaviorTreePaths = {
    "/root/autodl-tmp/linux_c++/message_process/behavior_tree_style1",
    "/root/autodl-tmp/linux_c++/message_process/behavior_tree_style2"
};

// 根据命令信息执行相应的行为树，并返回执行的行为树路径
std::string executeBehaviorTree(const Command& command) {
    if (command.behaviorTreeID < behaviorTreePaths.size()) {
        const std::string& behaviorTreePath = behaviorTreePaths[command.behaviorTreeID];
        // 这里执行行为树程序，您可以使用之前提供的函数来执行二进制程序
        std::cout << "Executing behavior tree at path: " << behaviorTreePath << std::endl;
        return behaviorTreePath;
    } else {
        std::cerr << "Invalid behavior tree ID: " << command.behaviorTreeID << std::endl;
        return ""; // 返回空字符串表示未找到匹配的行为树
    }
}

int main() {
    const char* argument1 = "1";        // 二进制程序1的参数

    const char* binary2Path = "/root/autodl-tmp/linux_c++/message_process/process_exit_example"; // 二进制程序2的路径
    const char* argument2 = "2";        // 二进制程序2的参数
    const char* name = "message_process";
    // 假设接收到的消息内容包含动作和行为树ID
    std::string action = "run";
    unsigned int behaviorTreeID = 0; // 假设行为树ID为0

    // 构造命令信息
    Command cmd;
    cmd.action = action;
    cmd.behaviorTreeID = behaviorTreeID;

    // 执行相应的行为树，并获取执行的行为树路径
    std::string binary1Path = executeBehaviorTree(cmd); // 将binary1Path改为std::string类型

    std::cout << "Executed behavior tree path: " << binary1Path << std::endl;
    const char* binary1PathChar = binary1Path.c_str();

    int result1 = runBinaryWithArgument(binary1PathChar, name, argument1);
    //int result2 = runBinaryWithArgument(binary2Path, name, argument2);

    return 0;
}
