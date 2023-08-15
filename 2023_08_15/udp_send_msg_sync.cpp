#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "msg_parse.h"

void send_msg(const sockaddr_in* dest, const char* buffer, size_t len) {

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    std::cout << "send_msg len=" << len << "\n";
    if (socket_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (sendto(socket_fd, buffer, len, 0, (const sockaddr*)dest, sizeof(*dest)) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    close(socket_fd);
}

int main() {
    sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &dest.sin_addr);

    // 发送 MSG_REQUIRE
    {
        MsgRequire require_msg = {{ {"key", "value"} }};
        std::string msg_serialized = require_msg.data.dump();
        uint32_t data_len = msg_serialized.size();
        uint32_t data_len_n = htonl(data_len); // 转为网络字节序

        MsgHeader header = {MSG_REQUIRE, data_len_n};
        char buffer[sizeof(MsgHeader) + data_len];
        memcpy(buffer, &header, sizeof(MsgHeader));
        memcpy(buffer + sizeof(MsgHeader), msg_serialized.c_str(), data_len);

        send_msg(&dest, buffer, sizeof(buffer));
    }

    // 发送 MSG_FILE
    {
        MsgFile file_msg = {"path/to/file.txt", 12345};
        uint32_t file_size_n = htonl(file_msg.file_size); // 转为网络字节序
        uint32_t data_len = file_msg.file_path.size() + sizeof(file_msg.file_size);
        uint32_t data_len_n = htonl(data_len); // 转为网络字节序

        MsgHeader header = {MSG_FILE, data_len_n};
        char buffer[sizeof(MsgHeader) + data_len];
        memcpy(buffer, &header, sizeof(MsgHeader));
        memcpy(buffer + sizeof(MsgHeader), file_msg.file_path.c_str(), file_msg.file_path.size());
        memcpy(buffer + sizeof(MsgHeader) + file_msg.file_path.size(), &file_size_n, sizeof(file_msg.file_size));

        send_msg(&dest, buffer, sizeof(buffer));
    }

    return 0;
}
