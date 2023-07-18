#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 封装的函数，启动一个二进制程序并传递一个参数
int runBinaryWithArgument(const char* binaryPath, const char* argument) {
    pid_t pid = fork();
    if (pid == 0) {
        // 在子进程中执行二进制程序
        execl(binaryPath, binaryPath, argument, nullptr);
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

int main() {
    const char* binary1Path = "/root/autodl-tmp/linux_c++/message_process"; // 二进制程序1的路径
    const char* argument1 = "1";        // 二进制程序1的参数

    const char* binary2Path = "/root/autodl-tmp/linux_c++/message_process"; // 二进制程序2的路径
    const char* argument2 = "2";        // 二进制程序2的参数

    int result1 = runBinaryWithArgument(binary1Path, argument1);
    int result2 = runBinaryWithArgument(binary2Path, argument2);

    // 对于二进制程序1和二进制程序2的执行结果，您可以根据需要进行处理

    return 0;
}