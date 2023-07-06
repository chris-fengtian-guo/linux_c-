#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void start_receiving(tcp::socket &socket, const std::string & file_name) {
	std::ofstream file(file_name, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file\n";
		return;
	}
	boost::asio::streambuf buf;
	boost::asio::read(socket, buf);
	std::istream in_stream(&buf);
	file << in_stream.rdbuf();
}
int main() {
	try {
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "1234");
		
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		start_receiving(socket, "received_file.txt");


	} catch (std::exception &e) {
		std::cerr << "Exceptioin:" << e.what() << "\n";
	}
	return 0;
}
