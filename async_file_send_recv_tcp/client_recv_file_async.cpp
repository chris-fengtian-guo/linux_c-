#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <boost/asio.hpp>
#include <openssl/md5.h>

using boost::asio::ip::tcp;

std::string get_file_md5(const std::string& file_name) {
    // ... The same MD5 function as before ...
}

void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred, std::size_t total_bytes,
    std::ofstream& file, tcp::socket& socket, std::vector<char>& buffer) {
    if (error) {
        std::cerr << "Failed to receive file: " << error.message() << "\n";
    } else {
        file.write(buffer.data(), bytes_transferred);
        if (file.fail()) {
            std::cerr << "Failed to write to the file.\n";
            return;
        }
        if (bytes_transferred != total_bytes) {
            socket.async_read_some(boost::asio::buffer(buffer),
                std::bind(handle_read, std::placeholders::_1, std::placeholders::_2, total_bytes, std::ref(file), std::ref(socket), std::ref(buffer)));
        } else {
            std::cout << "File received successfully.\n";
        }
    }
}

void receive_file(tcp::socket& socket, const std::string& file_name, std::size_t total_bytes) {
    std::ofstream file(file_name, std::ios::binary | std::ios::trunc);

    if (!file.is_open() || file.fail()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    std::vector<char> md5(32);
    boost::asio::read(socket, boost::asio::buffer(md5));

    std::vector<char> buffer(1024);
    socket.async_read_some(boost::asio::buffer(buffer),
        std::bind(handle_read, std::placeholders::_1, std::placeholders::_2, total_bytes, std::ref(file), std::ref(socket), std::ref(buffer)));
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("localhost", "1234");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // You need to know the total file size in order to receive the file asynchronously.
        // You can first synchronize to receive the file size, or you can determine it in another way.
        std::size_t total_bytes = /*...*/;

        receive_file(socket, "new_received_file.bin", total_bytes);

        // Run the IO service to start asynchronous operations.
        io_service.run();
    }
}
