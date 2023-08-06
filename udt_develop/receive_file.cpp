#include <iostream>
#include <udt.h>
#include <arpa/inet.h>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_to_save>\n";
        return 1;
    }

    UDTSOCKET serv = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(9000);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (UDT::ERROR == UDT::bind(serv, (sockaddr*)&my_addr, sizeof(my_addr))) {
        std::cerr << "bind: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return 1;
    }

    std::cout << "Listening on port 9000...\n";
    UDT::listen(serv, 10);

    int namelen = sizeof(my_addr);
    UDTSOCKET recver = UDT::accept(serv, (sockaddr*)&my_addr, &namelen);
    if (recver == UDT::INVALID_SOCK) {
        std::cerr << "accept: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return 1;
    }

    std::cout << "Accepted connection from " << inet_ntoa(my_addr.sin_addr) << ":" << ntohs(my_addr.sin_port) << "\n";

    std::ofstream ofs(argv[1], std::ios::out | std::ios::binary);

    char buffer[8192];
    int read;
 while (true) {
    read = UDT::recv(recver, buffer, sizeof(buffer), 0);
    if (read == 0) {
        std::cout << "File transfer completed. Connection closed by sender.\n";
        break;
    }
    if (read < 0) {
        std::cerr << "recv: " << UDT::getlasterror().getErrorMessage() << std::endl;
        break;
    }
    ofs.write(buffer, read);
    std::cout << "Received " << read << " bytes\n";
 }

    if (read < 0) {
        std::cerr << "recv: " << UDT::getlasterror().getErrorMessage() << std::endl;
    }

    UDT::close(recver);
    UDT::close(serv);
    std::cout << "File received successfully.\n";

    return 0;
}
