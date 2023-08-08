#include <iostream>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

const int HEADER_SIZE = 4;  

class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port)
        : socket_(io_service, udp::endpoint(udp::v4(), port)),
          total_size_(0), received_size_(0) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    if (total_size_ == 0 && bytes_transferred >= HEADER_SIZE) {
                        
                        total_size_ = *reinterpret_cast<int*>(recv_buffer_.data());
                        received_size_ += bytes_transferred - HEADER_SIZE;
			std::cout << "init receive bytes=" << bytes_transferred << "\n";
                    } else {
                        received_size_ += bytes_transferred;
			std::cout << "middle receive bytes=" << bytes_transferred << "\n";
                    }

                    if (received_size_ >= total_size_) {
                        std::cout << "Received total data of size: " << received_size_ << std::endl;
                        socket_.close();
                    } else {
                        start_receive();
                    }
                }
            });
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 65536> recv_buffer_;  
    int total_size_;
    int received_size_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        UDPServer server(io_service, std::stoi(argv[1]));

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
