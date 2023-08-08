void handle_write(const boost::system::error_code& ,
    size_t )
{
}

void start_sending_async(tcp::socket& socket, const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    boost::asio::streambuf buf;
    std::ostream out_stream(&buf);
    out_stream << file.rdbuf();
    
    
    boost::asio::async_write(socket, buf, handle_write);
}

