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

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    
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
        
        int numBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (numBytes < 0) {
            perror("recvfrom");
            continue;
        }

        
        std::string message(buffer, numBytes);
        std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr) << ": " << message << std::endl;
    }

    close(sockfd);
}

int main() {
    
    std::thread receiverThread(receiveMessages);

    
    receiverThread.join();

    return 0;
}
