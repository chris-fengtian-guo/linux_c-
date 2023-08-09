#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

std::queue<std::string> messageQueue;
std::mutex queueMutex;
std::condition_variable dataAvailable;

void udp_receiver() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAX_BUFFER_SIZE];

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &len);
        buffer[n] = '\0';

        std::unique_lock<std::mutex> lock(queueMutex);
	std::cout << "recvfrom Received Message: " << buffer << std::endl;
        messageQueue.push(std::string(buffer));
        dataAvailable.notify_one();
    }

    close(sockfd);
}

void message_processor() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        dataAvailable.wait(lock, [] { return !messageQueue.empty(); });

        std::string msg = messageQueue.front();
        messageQueue.pop();

        
        std::cout << "queue Received Message: " << msg << std::endl;
    }
}

int main() {
    std::thread receiverThread(udp_receiver);
    std::thread processorThread(message_processor);

    receiverThread.join();
    processorThread.join();

    return 0;
}
