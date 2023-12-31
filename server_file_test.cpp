#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void start_sending(tcp::socket &socket, const std::string &file_name) {
	std::ifstream file(file_name, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open the file\n";
		return;
	}
	file.seekg(0, std::ios::end);
	
	file.seekg(0, std::ios::beg);

	boost::asio::streambuf buf;
	std::ostream out_stream(&buf);
	std::cerr << "receive file content"; 
	out_stream << file.rdbuf();
	boost::asio::write(socket, buf);	
}
int main() {
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
		for (;;) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			start_sending(socket, "file_to_send.txt");
		}
	} catch(std::exception &e) {
			std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
