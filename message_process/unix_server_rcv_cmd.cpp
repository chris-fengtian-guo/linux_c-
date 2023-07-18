#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// 定义命令信息数据结构
struct Command {
    std::string action; // 动作
    int behaviorTreeID; // 行为树ID
};

int main() {
    // 创建Unix域套接字
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/unix_socket_server"); // 服务器的Unix域套接字文件路径

    // 绑定套接字到指定地址
    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // 监听连接
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Waiting for connection..." << std::endl;

    sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // 接受客户端连接
    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_addr_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    // 接收命令信息
    Command cmd;
    if (recv(client_fd, &cmd, sizeof(cmd), 0) == -1) {
        perror("recv");
    } else {
        // 解析命令信息
        std::cout << "Received command: " << cmd.action << " " << cmd.behaviorTreeID << std::endl;
    }

    // 关闭套接字
    close(client_fd);
    close(server_fd);

    return 0;
}
