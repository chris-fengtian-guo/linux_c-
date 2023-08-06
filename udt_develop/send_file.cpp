#include <iostream>
#include <udt.h>
#include <arpa/inet.h>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <file_to_send>\n";
        return 1;
    }

    UDTSOCKET client = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    std::cout << "Connecting to " << argv[1] << ":9000...\n";

    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        std::cerr << "connect: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return 1;
    }

    std::cout << "Connected to " << argv[1] << ":9000\n";

    std::ifstream ifs(argv[2], std::ios::in | std::ios::binary);

    char buffer[8192];
    int sent;
    while (ifs.read(buffer, sizeof(buffer))) {
        sent = UDT::send(client, buffer, sizeof(buffer), 0);
        if (sent < 0) {
            std::cerr << "send: " << UDT::getlasterror().getErrorMessage() << std::endl;
            break;
        }

        std::cout << "Sent " << sent << " bytes\n";
    }

    // 为了确保发送最后一部分（可能不足8192字节）
    sent = UDT::send(client, buffer, ifs.gcount(), 0);
    if (sent < 0) {
        std::cerr << "send: " << UDT::getlasterror().getErrorMessage() << std::endl;
    } else {
        std::cout << "Sent " << sent << " bytes\n";
    }

    UDT::close(client);
    std::cout << "File sent successfully.\n";

    return 0;
}
