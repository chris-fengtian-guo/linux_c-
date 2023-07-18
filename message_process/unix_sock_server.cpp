// Unix套接字服务器示例代码

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    // 创建套接字
    int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    // 配置服务器套接字地址
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, "/tmp/my_socket", sizeof(serverAddr.sun_path) - 1);

    // 绑定套接字
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to bind server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 监听连接
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen on server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 接受连接
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 数据通信
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        std::cout << "Received: " << buffer << std::endl;
        // 处理接收到的数据

        // 发送响应
        const char* response = "Hello, client!";
        ssize_t bytesSent = send(clientSocket, response, strlen(response), 0);
        if (bytesSent == -1) {
            std::cerr << "Failed to send response" << std::endl;
        }
    }

    // 关闭套接字
    close(clientSocket);
    close(serverSocket);

    // 删除套接字文件
    unlink("/tmp/my_socket");

    return 0;
}
