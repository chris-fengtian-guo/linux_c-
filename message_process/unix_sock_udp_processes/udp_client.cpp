#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>

using boost::asio::ip::udp;

const int PACKET_SIZE = 1024;
const int TOTAL_DATA_SIZE = 12000;  // Total size of the data to be sent

class UDPClient {
public:
    UDPClient(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
        : socket_(io_service) {
        udp::resolver resolver(io_service);
        udp::endpoint receiver_endpoint = *resolver.resolve({udp::v4(), host, port});
        socket_.open(udp::v4());

        std::string data(TOTAL_DATA_SIZE, 'x');  // Create a string of 12000 characters
        std::vector<char> send_buffer;

        // Insert data length at the beginning of the buffer
        int data_length = static_cast<int>(data.size());
        for (int i = 0; i < sizeof(data_length); ++i) {
            send_buffer.push_back(reinterpret_cast<char*>(&data_length)[i]);
        }

        // Insert the actual data
        for (const auto& ch : data) {
            send_buffer.push_back(ch);
        }

        // Send data in packets of PACKET_SIZE
        for (size_t i = 0; i < send_buffer.size(); i += PACKET_SIZE) {
            std::vector<char> packet(send_buffer.begin() + i, 
                send_buffer.begin() + std::min(send_buffer.size(), i + PACKET_SIZE));
            socket_.send_to(boost::asio::buffer(packet), receiver_endpoint);
        }

        // Start asynchronous receive for receipt from the server
        start_receive();
    }

private:
    void start_receive() {
	    std::cout << "received receipt\n";
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string receipt(recv_buffer_.data(), bytes_transferred);
                    std::cout << "Receipt: " << receipt << std::endl;
                }
                socket_.close();
            });
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 128> recv_buffer_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        UDPClient client(io_service, argv[1], argv[2]);

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
