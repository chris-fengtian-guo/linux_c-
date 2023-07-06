#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>  // add this
#include <boost/asio.hpp>
#include <openssl/md5.h>

#include <iostream>
#include <fstream>
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

void send_file(tcp::socket& socket, const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary);

    // Check if the file was opened successfully.
    if (!file.is_open() || file.fail()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    // Get the file size.
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file into memory.
    std::vector<char> file_data(size);
    if (!file.read(file_data.data(), size)) {
        std::cerr << "Failed to read the file.\n";
        return;
    }

    // Calculate the MD5 of the file.
    std::string md5 = get_file_md5(file_name);
    std::cout << "MD5 of the file to be sent: " << md5 << std::endl;
    std::cout << "Size of the file to be sent: " << size << " bytes\n";

    // Send the MD5 to the client.
    boost::asio::write(socket, boost::asio::buffer(md5));

    // Then send the file data.
    std::size_t written = boost::asio::write(socket, boost::asio::buffer(file_data));

    std::cout << "Written bytes: " << written << "\n";
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
        for (;;) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            send_file(socket, "file_to_send.bin");  // Replace with your file path.
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
