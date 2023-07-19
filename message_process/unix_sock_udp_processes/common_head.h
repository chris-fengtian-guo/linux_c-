#ifndef COMMAND_H
#define COMMAND_H
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>

#define UNIX_SRV_PATH "/tmp/my_socket"


#include <boost/serialization/access.hpp>

struct Command {
    unsigned int action; // 动作
    unsigned int behaviorTreeID; // 行为树ID

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & action;
        ar & behaviorTreeID;
    }
};

const int HEADER_SIZE = sizeof(int);  // 4 bytes = 32 bits

using boost::asio::ip::udp;
// Define a function type for the callback
using DataReceivedCallback = std::function<void(const Command&)>;

class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port, bool receipt_mode = false, DataReceivedCallback callback = nullptr)
        : socket_(io_service, udp::endpoint(udp::v4(), port)),
          total_size_(0), received_size_(0), receipt_mode_(receipt_mode), callback_(callback) {
        start_receive();
    }
    void send_data(const Command& cmd) {
        std::ostringstream archiveStream;
        boost::archive::text_oarchive archive(archiveStream);
        archive << cmd;
        std::string serialized_cmd = archiveStream.str();

        int total_length = HEADER_SIZE + serialized_cmd.size();
        std::string message = std::string(reinterpret_cast<char*>(&total_length), HEADER_SIZE) + serialized_cmd;

        boost::system::error_code ec;
        socket_.send_to(boost::asio::buffer(message), remote_endpoint_, 0, ec);

        if (!ec) {
            std::cout << "Sent data successfully\n";
        } else {
            std::cerr << "Failed to send data: " << ec.message() << "\n";
        }
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    if (total_size_ == 0 && bytes_transferred >= HEADER_SIZE) {
                        total_size_ = *reinterpret_cast<int*>(recv_buffer_.data());
                        //received_size_ += bytes_transferred - HEADER_SIZE;
			received_size_ += bytes_transferred;
			std::cout << "received_size_=" << received_size_ << "bytes_transferred=" << bytes_transferred << " total_size_=" << total_size_ << "\n";
                    } else {
                        received_size_ += bytes_transferred;
			std::cout << "received_size_=" << received_size_ << "bytes_transferred=" << bytes_transferred << " total_size_=" << total_size_ << "\n";
                    }

                    if (received_size_ >= total_size_) {
                        std::istringstream archiveStream(std::string(recv_buffer_.data() + HEADER_SIZE, bytes_transferred - HEADER_SIZE));
                        boost::archive::text_iarchive archive(archiveStream);
                        Command cmd;
                        archive >> cmd;
			std::cout << "received_size_=" << received_size_ << " total_size_=" << total_size_ << "\n";
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

#endif // COMMAND_H
