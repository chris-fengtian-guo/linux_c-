#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include "msg_parse.h"

const size_t MAX_BUFFER_SIZE = 1024;

void receive_and_parse_msg(int socket_fd) {
    char buffer[MAX_BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    ssize_t bytes_received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_len);
    if (bytes_received < 0) {
        perror("Receive failed");
        return;
    }
    std::cout << "total received=" << bytes_received << "\n";
    MsgHeader* header = reinterpret_cast<MsgHeader*>(buffer);

    switch (header->msg_type) {
        case MSG_REQUIRE: {
            std::string msg_serialized(buffer + sizeof(MsgHeader), ntohl(header->data_len));
            MsgRequire msg = {nlohmann::json::parse(msg_serialized)};
            std::cout << "Received MSG_REQUIRE with JSON data: " << msg.data.dump() << std::endl;
            break;
        }
        case MSG_FILE: {
            std::string file_path(buffer + sizeof(MsgHeader), ntohl(header->data_len) - sizeof(MsgFile::file_size));
            uint32_t file_size;
            memcpy(&file_size, buffer + sizeof(MsgHeader) + file_path.size(), sizeof(MsgFile::file_size));
            file_size = ntohl(file_size); // Convert to host byte order
            MsgFile msg = {file_path, file_size};
            std::cout << "Received MSG_FILE with path: " << msg.file_path << " and size: " << msg.file_size << std::endl;
            break;
        }
        default:
            std::cerr << "Unknown message type received" << std::endl;
            break;
    }
}

int main() {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(8080);
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_fd, (const sockaddr*)&local, sizeof(local)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 2; ++i) { // 两次接收
        receive_and_parse_msg(socket_fd);
    }

    close(socket_fd);

    return 0;
}
