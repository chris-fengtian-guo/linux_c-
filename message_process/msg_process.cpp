#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

void monitorBehaviorTreeProcess(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);  // 等待行为树进程结束
    if (WIFEXITED(status)) {
        int exitCode = WEXITSTATUS(status);
        std::cout << "Behavior Tree process exited with code: " << exitCode << std::endl;
    } else if (WIFSIGNALED(status)) {
        int signalNumber = WTERMSIG(status);
        std::cout << "Behavior Tree process terminated by signal: " << strsignal(signalNumber) << std::endl;
    } else {
        std::cout << "Behavior Tree process exited abnormally" << std::endl;
    }
}

int main() {
    // 启动行为树进程
    pid_t pid = fork();
    if (pid == 0) {
        // 在子进程中执行行为树进程
        // TODO: 替换为您的行为树进程启动代码
        execl("/usr/bin/df", "behavior_tree_process", nullptr);
        exit(0);
    } else if (pid > 0) {
        // 在父进程中监控行为树进程
        std::thread monitorThread(monitorBehaviorTreeProcess, pid);

        // 假设您的命令管理模块需要继续执行其他任务
        // 这里可以加入您的其他代码

        // 等待线程结束
        monitorThread.join();
    } else {
        std::cerr << "Failed to fork a new process" << std::endl;
    }

    return 0;
}
