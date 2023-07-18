#include <iostream>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

const int HEADER_SIZE = 4;  // 4 bytes = 32 bits

class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port, bool receipt_mode = false)
        : socket_(io_service, udp::endpoint(udp::v4(), port)),
          total_size_(0), received_size_(0), receipt_mode_(receipt_mode) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    if (total_size_ == 0 && bytes_transferred >= HEADER_SIZE) {
                        // Parse the header to get the total size
                        total_size_ = *reinterpret_cast<int*>(recv_buffer_.data());
                        received_size_ += bytes_transferred - HEADER_SIZE;
                    } else {
                        received_size_ += bytes_transferred;
                    }

                    if (received_size_ >= total_size_) {
                        std::cout << "Received total data of size: " << received_size_ << std::endl;
                        if (receipt_mode_) {
			    std::cout << "receipt_mode enable \n";
                            send_receipt();
                        } else {
			    std::cout << "not receipt close socket\n";
                            socket_.close();
                        }
                    } else {
                        start_receive();
                    }
                }
            });
    }

    void send_receipt() {
        std::string receipt = "Received: " + std::to_string(received_size_) + " Sent: " + std::to_string(total_size_);
	std::cout << "send receipt =" << receipt << "\n";
        socket_.async_send_to(
            boost::asio::buffer(receipt), remote_endpoint_,
            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_transferred*/) {
                socket_.close();
            });
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 65536> recv_buffer_;  // max UDP size
    int total_size_;
    int received_size_;
    bool receipt_mode_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: server <port> <receipt_mode>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        UDPServer server(io_service, std::stoi(argv[1]), std::stoi(argv[2]) != 0);

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
