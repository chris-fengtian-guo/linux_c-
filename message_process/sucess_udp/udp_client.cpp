#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;

    // 创建UDP套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // 设置服务器地址信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    while (true) {
        std::string message;

        // 输入消息
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        // 发送消息
        int numBytes = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (numBytes < 0) {
            perror("sendto");
            close(sockfd);
            return 1;
        }
    }

    close(sockfd);

    return 0;
}
