#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>

using boost::asio::ip::udp;

const int PACKET_SIZE = 1024;
const int TOTAL_DATA_SIZE = 12000;  

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::endpoint receiver_endpoint = *resolver.resolve({udp::v4(), argv[1], argv[2]});
        udp::socket socket(io_service);
        socket.open(udp::v4());

        std::string data(TOTAL_DATA_SIZE, 'x');  
        std::vector<char> send_buffer;

        
        int data_length = static_cast<int>(data.size());
        for (int i = 0; i < sizeof(data_length); ++i) {
            send_buffer.push_back(reinterpret_cast<char*>(&data_length)[i]);
        }

        
        for (const auto& ch : data) {
            send_buffer.push_back(ch);
        }

        
        for (size_t i = 0; i < send_buffer.size(); i += PACKET_SIZE) {
            std::vector<char> packet(send_buffer.begin() + i, 
                send_buffer.begin() + std::min(send_buffer.size(), i + PACKET_SIZE));
            socket.send_to(boost::asio::buffer(packet), receiver_endpoint);
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
