#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector> // 添加vector头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define MAX_CLIENTS 10

#include <boost/process.hpp>
#include <iostream>
#include "common_head.h"
namespace bp = boost::process;

bp::child runBinaryWithArgument2(const char* binaryPath, const char* argument) {
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

void* handleClient(void* clientSocket) {
    int client = *(int*)clientSocket;
    int bytesRead;
    //char buffer[256];
    //memset(buffer, 0, sizeof(buffer));

    Command cmd;
    bytesRead = recv(client, &cmd, sizeof(cmd), 0);
    if (bytesRead == -1) {
        perror("recv");
    } else {
        // 解析命令信息
	std::cout << "receive len=" << bytesRead << "\n";
        std::cout << "Received command: " << cmd.action << " " << cmd.behaviorTreeID << std::endl;
    	// 发送响应
        const char* response = "Hello, client!";
        //send(client, response, strlen(response), 0);
    }
    close(client);
    free(clientSocket);

    if (bytesRead > 0) {
	const char* argument1 = "1";        // 二进制程序1的参数
	/*
        const char* binary2Path = "/root/autodl-tmp/linux_c++/message_process/process_exit_example"; // 二进制程序2的路径
        const char* argument2 = "2";        // 二进制程序2的参数
        const char* name = "message_process";
	*/
	// 执行相应的行为树，并获取执行的行为树路径
    	std::string binary1Path = executeBehaviorTree(cmd); // 将binary1Path改为std::string类型

    	std::cout << "Executed behavior tree path: " << binary1Path << std::endl;
    	const char* binary1PathChar = binary1Path.c_str();
    	auto child = runBinaryWithArgument2(binary1PathChar, argument1);
    	//int result1 = runBinaryWithArgument(binary1PathChar, binary1PathChar, argument1);

    } 
    return NULL;
}

int run_unix_server() {
    // 创建套接字
    unlink(UNIX_SRV_PATH);
    int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create server socket");
        return 1;
    }

    // 配置服务器套接字地址
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, UNIX_SRV_PATH, sizeof(serverAddr.sun_path) - 1);

    // 绑定套接字
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to bind server socket");
        close(serverSocket);
        return 1;
    }

    // 监听连接
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Failed to listen on server socket");
        close(serverSocket);
        return 1;
    }

    while (1) {
        // 接受连接
        int* clientSocket = (int*)malloc(sizeof(int));
        if (clientSocket == NULL) {
            perror("Failed to allocate memory");
            continue;
        }
        *clientSocket = accept(serverSocket, NULL, NULL);
        if (*clientSocket == -1) {
            perror("Failed to accept client connection");
            free(clientSocket);
            continue;
        }

        // 创建线程处理客户端请求
        pthread_t tid;
        if (pthread_create(&tid, NULL, handleClient, clientSocket) != 0) {
            perror("Failed to create thread");
            close(*clientSocket);
            free(clientSocket);
            continue;
        }
        pthread_detach(tid);
    }

    close(serverSocket);
    unlink(UNIX_SRV_PATH);

    return 0;
}
int main() {
    run_unix_server();
    /*
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
    */
    return 0;
}
