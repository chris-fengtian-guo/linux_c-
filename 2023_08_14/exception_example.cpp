#include <stdexcept>

class NetworkException : public std::runtime_error {
public:
    explicit NetworkException(const std::string& msg) : std::runtime_error(msg) {}
    virtual ~NetworkException() {}
};

class UDPSendException : public NetworkException {
public:
    explicit UDPSendException(const std::string& msg) : NetworkException("UDP Send Error: " + msg) {}
};

class UDPReceiveException : public NetworkException {
public:
    explicit UDPReceiveException(const std::string& msg) : NetworkException("UDP Receive Error: " + msg) {}
};

void udp_send_function(/*...*/) {
    try {
        // ... 发送消息的代码
        if (some_error_condition) {
            throw UDPSendException("Failed to send the message due to XYZ");
        }
    } catch (const UDPSendException& e) {
        std::cerr << e.what() << std::endl;
        // ... 你可能想重新尝试、记录错误或通知用户
    }
}

void udp_receive_function(/*...*/) {
    try {
        // ... 接收消息的代码
        if (another_error_condition) {
            throw UDPReceiveException("Failed to receive the message due to XYZ");
        }
    } catch (const UDPReceiveException& e) {
        std::cerr << e.what() << std::endl;
        // ... 可以重新尝试、记录错误或通知用户
    }
}

void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    try {
        if (nread < 0) {
            throw UDPReceiveException("Error occurred while reading data");
        }
        // ... 其他处理代码
    } catch (const NetworkException& e) {
        std::cerr << e.what() << std::endl;
        // ... 可以重新尝试、记录错误或通知用户
    }
}
