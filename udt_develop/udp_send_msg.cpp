#include <iostream>
#include <udt.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void send_msg(const char* ip, int port, const std::string& message) {
    std::cout << "Creating UDT socket for sending..." << std::endl;
    UDTSOCKET client = UDT::socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    std::cout << "Trying to connect to " << ip << ":" << port << "..." << std::endl;
    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        std::cerr << "connect: " << UDT::getlasterror().getErrorMessage();
        return;
    }
    std::cout << "Connection established!" << std::endl;

    int msg_size = message.size();
    std::cout << "Sending message of size " << msg_size << " bytes..." << std::endl;
    if (UDT::ERROR == UDT::sendmsg(client, message.c_str(), msg_size)) {
        std::cerr << "sendmsg: " << UDT::getlasterror().getErrorMessage();
        return;
    }
    std::cout << "Message sent successfully!" << std::endl;

    UDT::close(client);
    std::cout << "Connection closed." << std::endl;
}

int main() {
    
    send_msg("127.0.0.1", 9000, "Hello, UDT!");
    return 0;
}
