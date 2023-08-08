#include <iostream>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>
#include <boost/asio.hpp>


#include <fstream>
#include <openssl/md5.h>
#include <fstream>

std::string get_file_size(const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open the file.\n";
        return {};
    }

    std::streamsize size = file.tellg();
    file.close();

    return std::to_string(size);
}
std::string get_md5_hash(const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open the file.\n";
        return {};
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    char buf[1024 * 16];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&md5Context, buf, file.gcount());
    }

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5Context);

    std::ostringstream sout;
    sout << std::hex << std::setfill('0');
    for (auto c : result) {
        sout << std::setw(2) << static_cast<int>(c);
    }

    return sout.str();
}
void start_sending(boost::asio::ip::tcp::socket& socket, const std::string& file_name) {

    std::string md5_hash = get_md5_hash(file_name);
    std::string file_size = get_file_size(file_name);

    
    std::string hash_and_size = md5_hash + "|" + file_size + "\n";

    boost::asio::write(socket, boost::asio::buffer(hash_and_size));

    
    
}
int main() {
    
    boost::asio::io_context io_context;

    
    boost::asio::ip::tcp::socket socket(io_context);

    
    
    socket.connect(endpoint);

    
    std::string file_name = "文件名";
    start_sending(socket, file_name);

    return 0;
}
