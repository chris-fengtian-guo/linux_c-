#include <iostream>
#include <udt.h>
#include <cstdlib>

int main() {
    // 初始化 UDT 库
    UDT::startup();

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(9000);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    // 创建 socket
    UDTSOCKET serv = UDT::socket(AF_INET, SOCK_DGRAM, 0);

    // 绑定地址
    if (UDT::ERROR == UDT::bind(serv, (sockaddr*)&my_addr, sizeof(my_addr))) {
        std::cerr << "bind: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    // 监听连接
    if (UDT::ERROR == UDT::listen(serv, 10)) {
        std::cerr << "listen: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    UDTSOCKET recver = UDT::accept(serv, (sockaddr*)&client_addr, &addrlen);

    char buffer[1024];
    int r = UDT::recvmsg(recver, buffer, sizeof(buffer));

    if (r < 0) {
        std::cerr << "recvmsg: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    std::cout << "Received message: " << buffer << std::endl;

    // 关闭连接
    UDT::close(recver);
    UDT::cleanup();

    return 0;
}
