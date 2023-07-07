#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <boost/asio.hpp>
#include <openssl/md5.h>
#include <boost/asio/buffer.hpp>

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

using boost::asio::ip::udp;

void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred, std::ifstream& file, uint32_t sequence_number, udp::socket& socket, udp::endpoint& endpoint, std::vector<char>& buffer) {
    if (error) {
        std::cerr << "Send error: " << error.message() << "\n";
        return;
    }

    if (!file.eof()) {
        sequence_number++;

        // Add sequence number to the buffer.
        std::memcpy(buffer.data(), &sequence_number, sizeof(sequence_number));

        // Read file data into the buffer, after the sequence number.
        file.read(buffer.data() + sizeof(sequence_number), buffer.size() - sizeof(sequence_number));
        std::streamsize data_read = file.gcount();

        if (!file) {
            std::cerr << "Read error or end of file.\n";
            return;
        }

        socket.async_send_to(boost::asio::buffer(buffer.data(), sizeof(sequence_number) + data_read), endpoint,
            std::bind(handle_send, std::placeholders::_1, std::placeholders::_2, std::ref(file), sequence_number, std::ref(socket), std::ref(endpoint), std::ref(buffer)));
    } else {
        std::cout << "File sent successfully.\n";
    }
}

void send_file(udp::socket& socket, udp::endpoint& endpoint, const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open() || file.fail()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    uint32_t sequence_number = 0;

    std::vector<char> buffer(1024 + sizeof(sequence_number));

    // Add sequence number to the buffer.
    std::memcpy(buffer.data(), &sequence_number, sizeof(sequence_number));

    // Read file data into the buffer, after the sequence number.
    file.read(buffer.data() + sizeof(sequence_number), buffer.size() - sizeof(sequence_number));
    std::streamsize data_read = file.gcount();

    socket.async_send_to(boost::asio::buffer(buffer.data(), sizeof(sequence_number) + data_read), endpoint,
        std::bind(handle_send, std::placeholders::_1, std::placeholders::_2, std::ref(file), sequence_number, std::ref(socket), std::ref(endpoint), std::ref(buffer)));
}

int main() {
    try {
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::v4());

        udp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234);

        send_file(socket, endpoint, "file_to_send.bin");

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
