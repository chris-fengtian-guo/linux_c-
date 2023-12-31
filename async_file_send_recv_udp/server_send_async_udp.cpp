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
#include <iomanip>  
#include <boost/asio.hpp>
#include <openssl/md5.h>

#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <openssl/md5.h>

using boost::asio::ip::udp;
void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred, std::ifstream& file, std::streamsize& total_read, std::streamsize total_length, uint32_t sequence_number, udp::socket& socket, udp::endpoint& endpoint, std::vector<char>& buffer) {
    if (!error && total_read < total_length) {
        sequence_number++;

        std::cout << "Sequence number: " << sequence_number << "\n";
        std::cout << "Total read: " << total_read << " / " << total_length << "\n";

        
        std::memcpy(buffer.data(), &sequence_number, sizeof(sequence_number));

        
        file.read(buffer.data() + sizeof(sequence_number), buffer.size() - sizeof(sequence_number));
        std::streamsize data_read = file.gcount();
        total_read += data_read;

        if (total_read <= total_length) {
            socket.async_send_to(boost::asio::buffer(buffer.data(), sizeof(sequence_number) + data_read), endpoint,
                std::bind(handle_send, std::placeholders::_1, std::placeholders::_2, std::ref(file), std::ref(total_read), total_length, sequence_number, std::ref(socket), std::ref(endpoint), std::ref(buffer)));
        } else {
            std::cout << "File sent successfully.\n";
        }
    } else {
        std::cerr << "Error while sending file: " << error.message() << "\n";
    }
}

void send_file(udp::socket& socket, udp::endpoint& endpoint, const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open() || file.fail()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    
    file.seekg(0, file.end);
    std::streamsize total_length = file.tellg();
    file.seekg(0, file.beg);

    std::cout << "File size: " << total_length << "\n";

    uint32_t sequence_number = 0;
    std::streamsize total_read = 0;

    std::vector<char> buffer(1024 + sizeof(sequence_number));

    
    std::memcpy(buffer.data(), &sequence_number, sizeof(sequence_number));

    
    file.read(buffer.data() + sizeof(sequence_number), buffer.size() - sizeof(sequence_number));
    std::streamsize data_read = file.gcount();
    total_read += data_read;

    std::cout << "Reading file: " << data_read << " bytes read.\n";

    if (total_read < total_length) {
        socket.async_send_to(boost::asio::buffer(buffer.data(), sizeof(sequence_number) + data_read), endpoint,
            std::bind(handle_send, std::placeholders::_1, std::placeholders::_2, std::ref(file), total_read, total_length, sequence_number, std::ref(socket), std::ref(endpoint), std::ref(buffer)));
    }
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
