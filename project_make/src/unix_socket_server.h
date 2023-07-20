#ifndef UNIX_SOCKET_SERVER_H
#define UNIX_SOCKET_SERVER_H

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <memory>

#define MAX_CLIENTS 10

class UnixSocketServer {
public:
    UnixSocketServer(const std::string& path);
    ~UnixSocketServer();
    void RunServer();
private:
    static void* HandleClient(void* clientSocket);
    std::string server_path_;
    int server_socket_;
};

#endif //UNIX_SOCKET_SERVER_H
