#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <sstream>

enum MessageType {
    FILE_TRANSFER,
    COMMAND_INFO,
    DEVICE_STATUS
};

struct Message {
    MessageType type;
    std::string uuid;
    std::string device_id;
    std::string content;
    
    static Message deserialize(const std::string& str) {
        Message msg;
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        msg.type = static_cast<MessageType>(std::stoi(tokens[0]));
        msg.uuid = tokens[1];
        msg.device_id = tokens[2];
        msg.content = tokens[3];
        
        return msg;
    }
};

void start_udp_server(int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    while (true) {
        char buffer[2048];
        socklen_t client_len = sizeof(client_addr);
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, 
                        (struct sockaddr*)&client_addr, &client_len);
        
        buffer[n] = '\0';
        
        Message msg = Message::deserialize(buffer);
        
        
        
        std::cout << "Received message type: " << msg.type << " UUID: " << msg.uuid << std::endl;
    }
    
    close(sockfd);
}

int main() {
    start_udp_server(12345);
    return 0;
}
