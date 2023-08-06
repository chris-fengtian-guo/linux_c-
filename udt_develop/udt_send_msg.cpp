#include <iostream>
#include <udt.h>
#include <cstdlib>
#include <cstring>

int main(int argc, char* argv[]) {
    // 初始化 UDT 库
    UDT::startup();

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);

    // 创建 socket
    UDTSOCKET client = UDT::socket(AF_INET, SOCK_DGRAM, 0);

    // 连接到服务器
    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        std::cerr << "connect: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    // 发送消息
    char* message = "Hello, UDT!";
    int msglen = strlen(message) + 1;  // 包括结束字符 '\0'
    if (UDT::ERROR == UDT::sendmsg(client, message, msglen)) {
        std::cerr << "sendmsg: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    // 关闭连接
    UDT::close(client);
    UDT::cleanup();

    return 0;
}
