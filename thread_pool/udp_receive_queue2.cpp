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
        
        MessageHeader header = { message.size(), calculate_md5(message), ++id_ };
        message = encode_header(header) + message;

        
        pending_confirmation_.insert({id_, header});

        boost::system::error_code ec;
        socket_.send_to(boost::asio::buffer(message), remote_endpoint_, 0, ec);

        if (!ec) {
            std::cout << "Sent command successfully\n";
        } else {
            std::cerr << "Failed to send command: " << ec.message() << "\n";
        }

        
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    Receipt receipt = decode_receipt(std::string(recv_buffer_.data(), bytes_transferred));
                    auto it = pending_confirmation_.find(receipt.id);
                    if (it != pending_confirmation_.end()) {
                        
                        if (check_receipt(it->second, receipt)) {
                            std::cout << "Receipt confirmed: " << receipt.id << "\n";
                            pending_confirmation_.erase(it);
                        } else {
                            std::cerr << "Receipt check failed: " << receipt.id << "\n";
                        }
                    }
                }
            });
    }

    

    std::map<int, MessageHeader> pending_confirmation_;
    int id_ = 0;
};
