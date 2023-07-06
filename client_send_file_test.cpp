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
    // Truncate the file if it already exists, create it if it does not.
    std::ofstream file(file_name, std::ios::binary | std::ios::trunc);

    // Check if the file was opened correctly.
    if (!file.is_open() || file.fail()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    // First receive the MD5 from the server.
    std::vector<char> md5(32);
    boost::asio::read(socket, boost::asio::buffer(md5));

    // Then receive the file data.
    std::vector<char> file_data(1024);
    size_t length;
    try {
        while ((length = socket.read_some(boost::asio::buffer(file_data))) > 0) {
            file.write(file_data.data(), length);
            
            // Check if the write was successful.
            if (file.fail()) {
                std::cerr << "Failed to write to the file.\n";
                return;
            }
        }
    } catch (const boost::system::system_error& ex) {
        // If we get an EOF error, it's expected, so don't report it.
        if (ex.code() != boost::asio::error::eof) {
            std::cerr << "Read error: " << ex.what() << "\n";
        }
    }

    // Verify the MD5 of the received file.
    std::string received_md5 = get_file_md5(file_name);

    // Print the received MD5 from server.
    std::string server_md5 = std::string(md5.begin(), md5.end());
    std::cout << "MD5 from server: " << server_md5 << std::endl;

    // Print the calculated MD5.
    std::cout << "Calculated MD5 of received file: " << received_md5 << std::endl;

    if (received_md5 == server_md5) {
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

        receive_file(socket, "new_received_file.bin");  // Replace with your file path.
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
