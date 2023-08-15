#include "msg_parse.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#define MAX_BUFFER_SIZE 1024

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

    char buffer[MAX_BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    ssize_t bytes_received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_len);
    if (bytes_received < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
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
	    std::cout << "MSG_FILE received=" << bytes_received << "\n";
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

    close(socket_fd);

    return 0;
}
