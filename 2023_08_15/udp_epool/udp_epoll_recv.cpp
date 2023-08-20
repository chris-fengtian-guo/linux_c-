#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>

constexpr int MAX_BUFFER_SIZE = 1024;
constexpr int PORT = 8080;
constexpr int MAX_EVENTS = 10;

int set_socket_nonblocking(int socket_fd) {
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    if (set_socket_nonblocking(socket_fd) < 0) {
        perror("Set socket non-blocking failed");
        close(socket_fd);
        return 1;
    }

    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_fd, (const sockaddr*)&local, sizeof(local)) < 0) {
        perror("Bind failed");
        close(socket_fd);
        return 1;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("Epoll create failed");
        close(socket_fd);
        return 1;
    }

    epoll_event event;
    event.data.fd = socket_fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) {
        perror("Epoll ctl add failed");
        close(epoll_fd);
        close(socket_fd);
        return 1;
    }

    epoll_event events[MAX_EVENTS];
    char buffer[MAX_BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (true) {
        int ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < ready; i++) {
            if (events[i].data.fd == socket_fd && events[i].events & EPOLLIN) {
                ssize_t bytes_received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_len);
                if (bytes_received > 0) {
                    std::cout << "Received: " << std::string(buffer, bytes_received) << std::endl;
                }
            }
        }
    }

    close(epoll_fd);
    close(socket_fd);
    return 0;
}
