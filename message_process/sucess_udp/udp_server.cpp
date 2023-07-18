#include <iostream>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>

void receiveMessages() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[1024];

    // 创建UDP套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // 绑定UDP套接字到本地IP和端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8888);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        close(sockfd);
        return;
    }

    std::cout << "UDP server listening on port 8888..." << std::endl;

    while (true) {
        // 接收消息
        int numBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (numBytes < 0) {
            perror("recvfrom");
            continue;
        }

        // 处理接收到的消息
        std::string message(buffer, numBytes);
        std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) << ": " << message << std::endl;
    }

    close(sockfd);
}

int main() {
    // 启动接收消息的线程
    std::thread receiverThread(receiveMessages);

    // 等待接收线程结束
    receiverThread.join();

    return 0;
}
