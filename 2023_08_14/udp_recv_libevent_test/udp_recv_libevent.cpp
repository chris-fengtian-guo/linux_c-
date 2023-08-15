#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <iostream>
#include <cstring>

// 全局变量
struct event_base *base;
int udpSocket;

// 定义错误处理函数
void udp_error_cb(int severity, const char *msg) {
    Logger::log(std::string("UDP Error (Severity: ") + std::to_string(severity) + "): " + msg);
}

// UDP接收回调
void udp_read_cb(evutil_socket_t fd, short events, void *arg) {
    char buffer[4096];
    sockaddr_in remote;
    socklen_t remote_len = sizeof(remote);

    int bytesReceived = recvfrom(fd, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&remote, &remote_len);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        // 处理接收到的数据 ...
    } else if (bytesReceived < 0) {
        Logger::log("Error while reading from UDP socket");
    }
}

int main() {
    // 初始化libevent
    event_set_log_callback(udp_error_cb);
    base = event_base_new();

    // 创建UDP套接字
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        Logger::log("Failed to create UDP socket");
        return 1;
    }

    sockaddr_in local;
    std::memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(12345);  // 例如监听12345端口
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(udpSocket, (sockaddr*)&local, sizeof(local)) == -1) {
        Logger::log("Failed to bind UDP socket");
        return 1;
    }

    // 设置套接字为非阻塞模式
    evutil_make_socket_nonblocking(udpSocket);

    // 添加事件监听UDP套接字
    event *udp_event = event_new(base, udpSocket, EV_READ | EV_PERSIST, udp_read_cb, nullptr);
    event_add(udp_event, nullptr);

    // 进入事件循环
    event_base_dispatch(base);

    // 清理资源
    event_free(udp_event);
    event_base_free(base);
    close(udpSocket);

    return 0;
}
