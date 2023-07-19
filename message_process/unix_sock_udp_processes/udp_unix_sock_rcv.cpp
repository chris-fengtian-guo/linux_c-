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
#include "common_head.h"

using boost::asio::ip::udp;

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }
	CommandHandler handler;

        boost::asio::io_service io_service;
        UDPServer udp_server(io_service, std::stoi(argv[1]), true, std::bind(&CommandHandler::handle_command, &handler, std::placeholders::_1));

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

        // ...
}

