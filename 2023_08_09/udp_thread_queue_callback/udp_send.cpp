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

#define PORT 8080
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
 
    std::string serialize() const {
    	std::stringstream ss;
    	ss << static_cast<int>(type) << ',' << uuid << ',' << device_id << ',' << content;
    	return ss.str();
    }   
};

void send_udp_message(const Message& msg, const char* server_ip, int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    
    std::string serialized_msg = msg.serialize();
    sendto(sockfd, serialized_msg.c_str(), serialized_msg.size(), 0, 
           (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    close(sockfd);
}

int main() {
    Message msg = {COMMAND_INFO, "1234-5678", "DEVICE_001", "Some command"};
    send_udp_message(msg, "127.0.0.1", PORT);
    return 0;
}
