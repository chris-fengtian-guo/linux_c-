#include <iostream>
#include <queue>
#include <mutex>
#include <cstring>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/listener.h>

#include "msg_hdr.h"
std::queue<MsgHeader> data_queue;
std::mutex mtx;

// The global base event
struct event_base* base;
struct event* udp_send_event;
evutil_socket_t socket_fd;

void send_msg(const sockaddr* dest, const char* buffer, size_t len) {
    sendto(socket_fd, buffer, len, 0, dest, sizeof(*dest));
}

void periodic_send_cb(evutil_socket_t fd, short what, void* arg) {
    // You can periodically send messages here
    // For now, this function does nothing, but it's here if you need it
}

int main() {
    base = event_base_new();
    
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1 in hex
    sin.sin_port = htons(12345);

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }
    
    udp_send_event = event_new(base, socket_fd, EV_WRITE|EV_PERSIST, periodic_send_cb, NULL);
    event_add(udp_send_event, NULL);

    // Sending MSG_REQUIRE
    {
        //MsgHeader header = {MSG_REQUIRE, sizeof(nlohmann::json)};
        MsgHeader header = {MSG_REQUIRE, 0};
        MsgRequire require_msg = {{ {"key", "value"} }};
        
        std::string msg_serialized = require_msg.data.dump();
        char buffer[sizeof(MsgHeader) + msg_serialized.size()];
        header.data_len = msg_serialized.size();
        memcpy(buffer, &header, sizeof(MsgHeader));
        memcpy(buffer + sizeof(MsgHeader), msg_serialized.c_str(), msg_serialized.size());

        send_msg((const sockaddr*)&sin, buffer, sizeof(buffer));
    }

    // Sending MSG_FILE
    {
        MsgHeader header = {MSG_FILE, sizeof(std::string) + sizeof(uint64_t)};
        MsgFile file_msg = {"path/to/file.txt", 12345};

        char buffer[sizeof(MsgHeader) + sizeof(std::string) + sizeof(uint64_t)];
        memcpy(buffer, &header, sizeof(MsgHeader));
        memcpy(buffer + sizeof(MsgHeader), file_msg.file_path.c_str(), file_msg.file_path.size());
        memcpy(buffer + sizeof(MsgHeader) + file_msg.file_path.size(), &file_msg.file_size, sizeof(uint64_t));

        send_msg((const sockaddr*)&sin, buffer, sizeof(buffer));
    }

    event_base_dispatch(base);

    event_free(udp_send_event);
    evutil_closesocket(socket_fd);
    event_base_free(base);

    return 0;
}
