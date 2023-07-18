// Unix套接字客户端示例代码

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    // 创建套接字
    int clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create client socket" << std::endl;
        return 1;
    }

    // 配置服务器套接字地址
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, "/tmp/my_socket", sizeof(serverAddr.sun_path) - 1);

    // 连接到服务器
    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        close(clientSocket);
        return 1;
    }

    // 数据通信
    const char* message = "Hello, server!";
    ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent == -1) {
        std::cerr << "Failed to send message" << std::endl;
    }

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        std::cout << "Received: " << buffer << std::endl;
        // 处理接收到的数据
    }

    // 关闭套接字
    close(clientSocket);

    return 0;
}
