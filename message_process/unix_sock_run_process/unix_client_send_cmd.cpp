#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include "common_head.h"
/*
// 定义命令信息数据结构
struct Command {
    std::string action; // 动作
    int behaviorTreeID; // 行为树ID
};
*/

int main() {
    // 创建Unix域套接字
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, UNIX_SRV_PATH); // 服务器的Unix域套接字文件路径

    // 连接到服务器
    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    // 假设接收到的消息内容包含动作和行为树ID
    //std::string action = "run";
    unsigned int action = 1;
    unsigned int behaviorTreeID = 0;

    // 构造命令信息
    Command cmd;
    cmd.action = action;
    cmd.behaviorTreeID = behaviorTreeID;
    std::cout << "send len=" << sizeof(cmd) << "\n";

    // 发送命令信息到服务器
    if (send(sockfd, &cmd, sizeof(cmd), 0) == -1) {
        perror("send");
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
