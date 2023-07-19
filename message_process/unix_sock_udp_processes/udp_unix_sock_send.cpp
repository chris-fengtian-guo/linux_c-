#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <string>
#include <iostream>

using boost::asio::ip::udp;

struct Command {
    unsigned int action; // 动作
    unsigned int behaviorTreeID; // 行为树ID

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & action;
        ar & behaviorTreeID;
    }
};

class UDPClient {
public:
    UDPClient(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
        : socket_(io_service), resolver_(io_service) {
        remote_endpoint_ = *resolver_.resolve({udp::v4(), host, port}).begin();
        socket_.open(udp::v4());
    }

    void send_cmd(Command cmd) {
        std::ostringstream archiveStream;
        boost::archive::text_oarchive archive(archiveStream);
        archive << cmd;
        std::string message = archiveStream.str();

        boost::system::error_code ec;
        socket_.send_to(boost::asio::buffer(message), remote_endpoint_, 0, ec);

        if (!ec) {
            std::cout << "Sent command successfully\n";
        } else {
            std::cerr << "Failed to send command: " << ec.message() << "\n";
        }

        // Start asynchronous receive for receipt from the server
        start_receive();
    }

private:
    void start_receive() {
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
    udp::resolver resolver_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        UDPClient client(io_service, argv[1], argv[2]);

        Command cmd;
        cmd.action = 1;
        cmd.behaviorTreeID = 2;
        client.send_cmd(cmd);

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
