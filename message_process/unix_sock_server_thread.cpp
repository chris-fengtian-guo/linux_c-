// Unix套接字服务器示例代码

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define MAX_CLIENTS 10

void* handleClient(void* clientSocket) {
    int client = *(int*)clientSocket;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesRead = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        printf("Received from client: %s\n", buffer);
        // 处理客户端请求

        // 发送响应
        const char* response = "Hello, client!";
        send(client, response, strlen(response), 0);
    }

    close(client);
    free(clientSocket);
    return NULL;
}

int main() {
    // 创建套接字
    int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create server socket");
        return 1;
    }

    // 配置服务器套接字地址
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, "/tmp/my_socket", sizeof(serverAddr.sun_path) - 1);

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
    unlink("/tmp/my_socket");

    return 0;
}
