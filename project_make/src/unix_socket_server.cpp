#include "unix_socket_server.h"
#include "command_handler.h"

UnixSocketServer::UnixSocketServer(const std::string& path) : server_path_(path), server_socket_(-1) {
    // 创建套接字
    unlink(server_path_.c_str());
    server_socket_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        perror("Failed to create server socket");
        throw std::runtime_error("Failed to create server socket");
    }

    // 配置服务器套接字地址
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, server_path_.c_str(), sizeof(serverAddr.sun_path) - 1);

    // 绑定套接字
    if (bind(server_socket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to bind server socket");
        close(server_socket_);
        throw std::runtime_error("Failed to bind server socket");
    }

    // 监听连接
    if (listen(server_socket_, MAX_CLIENTS) == -1) {
        perror("Failed to listen on server socket");
        close(server_socket_);
        throw std::runtime_error("Failed to listen on server socket");
    }
}

UnixSocketServer::~UnixSocketServer() {
    close(server_socket_);
    unlink(server_path_.c_str());
}

void UnixSocketServer::RunServer() {
    while (1) {
        // 接受连接
        int* clientSocket = new int;
        *clientSocket = accept(server_socket_, NULL, NULL);
        if (*clientSocket == -1) {
            perror("Failed to accept client connection");
            delete clientSocket;
            continue;
        }

        // 创建线程处理客户端请求
        pthread_t tid;
        if (pthread_create(&tid, NULL, HandleClient, clientSocket) != 0) {
            perror("Failed to create thread");
            close(*clientSocket);
            delete clientSocket;
            continue;
        }
        pthread_detach(tid);
    }
}

void* UnixSocketServer::HandleClient(void* clientSocket) {
    CommandHandler cmdHandler;
    cmdHandler.Handle(static_cast<int*>(clientSocket));
    return NULL;
}
