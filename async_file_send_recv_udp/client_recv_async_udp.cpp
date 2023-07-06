#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <map>

using boost::asio::ip::udp;

void handle_receive(size_t bytes_transferred, boost::system::error_code error,
                    udp::socket& socket, udp::endpoint& endpoint, std::vector<char>& buffer, std::ofstream& file, uint32_t& expected_sequence_number, std::map<uint32_t, std::vector<char>>& out_of_order_data) {
    if (error) {
        std::cerr << "Receive error: " << error.message() << "\n";
        return;
    }

    uint32_t sequence_number;
    std::memcpy(&sequence_number, buffer.data(), sizeof(sequence_number));

    if (sequence_number == expected_sequence_number) {
        file.write(buffer.data() + sizeof(sequence_number), bytes_transferred - sizeof(sequence_number));

        ++expected_sequence_number;

        // Check if we have any consecutive packets in the out_of_order_data.
        while (out_of_order_data.count(expected_sequence_number)) {
            file.write(out_of_order_data[expected_sequence_number].data(), out_of_order_data[expected_sequence_number].size());
            out_of_order_data.erase(expected_sequence_number);
            ++expected_sequence_number;
        }
    } else if (sequence_number > expected_sequence_number) {
        // Out of order packet. Save it for later.
        out_of_order_data[sequence_number] = std::vector<char>(buffer.data() + sizeof(sequence_number), buffer.data() + bytes_transferred);
    }

    // Continue receiving.
    socket.async_receive_from(boost::asio::buffer(buffer), endpoint, std::bind(handle_receive, std::placeholders::_1, std::placeholders::_2, std::ref(socket), std::ref(endpoint), std::ref(buffer), std::ref(file), std::ref(expected_sequence_number), std::ref(out_of_order_data)));
}

int main() {
    try {
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 1234));

        std::ofstream file("received_file.bin", std::ios::binary | std::ios::trunc);
        std::vector<char> buffer(1024 + sizeof(uint32_t));
        udp::endpoint endpoint;
        uint32_t expected_sequence_number = 0;
        std::map<uint32_t, std::vector<char>> out_of_order_data;

        socket.async_receive_from(boost::asio::buffer(buffer), endpoint, std::bind(handle_receive, std::placeholders::_1, std::placeholders::_2, std::ref(socket), std::ref(endpoint), std::ref(buffer), std::ref(file), std::ref(expected_sequence_number), std::ref(out_of_order_data)));

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
