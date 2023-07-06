#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>  // add this
#include <boost/asio.hpp>
#include <openssl/md5.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <boost/asio.hpp>
#include <openssl/md5.h>

using boost::asio::ip::tcp;

std::string get_file_md5(const std::string& file_name) {
    unsigned char result[MD5_DIGEST_LENGTH];
    std::ifstream file(file_name, std::ios::binary);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    MD5((unsigned char*)&buffer[0], buffer.size(), result);

    std::ostringstream sout;
    sout<<std::hex<<std::setfill('0');
    for(long long c: result) {
        sout<<std::setw(2)<<(long long)c;
    }
    return sout.str();
}

void receive_file(tcp::socket& socket, const std::string& file_name) {
    std::ofstream file(file_name, std::ios::binary);

    // First receive the MD5 from the server.
    std::vector<char> md5(32);
    boost::asio::read(socket, boost::asio::buffer(md5));

    // Then receive the file data.
    std::vector<char> file_data(1024);
    size_t length;
    while ((length = socket.read_some(boost::asio::buffer(file_data))) > 0) {
        file.write(file_data.data(), length);
    }

    // Verify the MD5 of the received file.
    std::string received_md5 = get_file_md5(file_name);
    if (received_md5 == std::string(md5.begin(), md5.end())) {
        std::cout << "File received correctly.\n";
    } else {
        std::cerr << "File received with errors.\n";
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("localhost", "1234");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        receive_file(socket, "received_file.bin");  // Replace with your file path.
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
