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
//void start_sending(tcp::socket& socket, const std::string& file_name) {
    std::string md5_hash = get_md5_hash(file_name);
    std::string file_size = get_file_size(file_name);

    // Assume the hash and the size are sent in the format "hash|size\n"
    std::string hash_and_size = md5_hash + "|" + file_size + "\n";

    boost::asio::write(socket, boost::asio::buffer(hash_and_size));

    // Proceed with sending the file.
    // ...
}
int main() {
    // 创建 Boost.Asio io_context 对象
    boost::asio::io_context io_context;

    // 创建 socket 对象
    boost::asio::ip::tcp::socket socket(io_context);

    // 连接到服务器
    //boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("服务器IP地址"), 服务器端口号);
    socket.connect(endpoint);

    // 调用 start_sending 函数
    std::string file_name = "文件名";
    start_sending(socket, file_name);

    return 0;
}
