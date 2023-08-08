#include <iostream>
#include <udt.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


void receive_msg(int port) {
    std::cout << "Creating UDT socket for receiving..." << std::endl;
    UDTSOCKET serv = UDT::socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    std::cout << "Binding to port " << port << "..." << std::endl;
    if (UDT::ERROR == UDT::bind(serv, (sockaddr*)&my_addr, sizeof(my_addr))) {
        std::cerr << "bind: " << UDT::getlasterror().getErrorMessage();
        return;
    }

    UDT::listen(serv, 10);
    std::cout << "Listening for connections..." << std::endl;

    sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    UDTSOCKET recver = UDT::accept(serv, (sockaddr*)&client_addr, &addrlen);
    std::cout << "Connection request received!" << std::endl;

    char client_ip[16];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    std::cout << "New connection from " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

    char* msg = new char[100];
    std::cout << "Waiting to receive a message..." << std::endl;
    int r = UDT::recvmsg(recver, msg, 100);
    if (r < 0) {
        std::cerr << "recvmsg: " << UDT::getlasterror().getErrorMessage();
        delete[] msg;
        return;
    }

    std::cout << "Received message: " << msg << std::endl;

    delete[] msg;
    UDT::close(recver);
    UDT::close(serv);
    std::cout << "Connection closed." << std::endl;
}

int main() {
    
    receive_msg(9000);
    return 0;
}
