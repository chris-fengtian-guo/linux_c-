#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <functional>
#include "common_head.h"
#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using boost::asio::ip::udp;

const int HEADER_SIZE = 4;  // 4 bytes = 32 bits


// Define a function type for the callback
using DataReceivedCallback = std::function<void(const Command&)>;

class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port, bool receipt_mode = false, DataReceivedCallback callback = nullptr)
        : socket_(io_service, udp::endpoint(udp::v4(), port)),
          total_size_(0), received_size_(0), receipt_mode_(receipt_mode), callback_(callback) {
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
                    } else {
                        received_size_ += bytes_transferred;
                    }

                    if (received_size_ >= total_size_) {
                        std::istringstream archiveStream(std::string(recv_buffer_.data() + HEADER_SIZE, bytes_transferred - HEADER_SIZE));
                        boost::archive::text_iarchive archive(archiveStream);
                        Command cmd;
                        archive >> cmd;

                        // Call the callback if it is set
                        if (callback_) {
                            callback_(cmd);
                        }

                        if (receipt_mode_) {
                            send_receipt();
                        } else {
                            socket_.close();
                        }
                    } else {
                        start_receive();
                    }
                }
            });
    }

    // send_receipt() stays the same ...
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
    DataReceivedCallback callback_;  // Add a callback for data received
};


class CommandHandler {
public:
    void handle_command(const Command& cmd) {
        std::cout << "Handle command with action: " << cmd.action << " and behaviorTreeID: " << cmd.behaviorTreeID << std::endl;
    }
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: server <port> <receipt_mode>\n";
            return 1;
        }
	CommandHandler handler;

        boost::asio::io_service io_service;
        UDPServer udp_server(io_service, 12345, true, std::bind(&CommandHandler::handle_command, &handler, std::placeholders::_1));

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

        // ...
}
