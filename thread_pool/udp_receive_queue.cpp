#include <boost/asio.hpp>
#include <iostream>

class UDPReceiver {
public:
    UDPReceiver(boost::asio::io_service& io_service)
        : socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(data_, max_length), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec && bytes_transferred > 0) {
                    handle_receive(ec, bytes_transferred);
                } else {
                    start_receive();
                }
            });
    }

    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
        
        std::cout << "Received: " << std::string(data_, bytes_transferred) << std::endl;
        start_receive(); 
    }

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    char data_[max_length];
    static constexpr int max_length = 1024;
};

int main() {
    try {
        boost::asio::io_service io_service;
        UDPReceiver receiver(io_service);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
