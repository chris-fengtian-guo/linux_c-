enum class ErrorCode {
    SUCCESS = 0,
    UDP_SEND_FAILURE,
    UDP_RECEIVE_FAILURE,
    // ... 其他错误
};

ErrorCode udp_send_function(/*...*/) {
    // ... 发送代码
    if (some_error_condition) {
        return ErrorCode::UDP_SEND_FAILURE;
    }
    return ErrorCode::SUCCESS;
}

class ErrorHandler {
public:
    static void handle(ErrorCode code) {
        switch (code) {
            case ErrorCode::SUCCESS:
                // No error
                break;
            case ErrorCode::UDP_SEND_FAILURE:
                std::cerr << "UDP send failed!" << std::endl;
                // ... 你可以重新尝试、记录错误或通知用户
                break;
            case ErrorCode::UDP_RECEIVE_FAILURE:
                std::cerr << "UDP receive failed!" << std::endl;
                // ... 你可以重新尝试、记录错误或通知用户
                break;
            // ... handle other errors
        }
    }
};

ErrorCode result = udp_send_function(/*...*/);
ErrorHandler::handle(result);

void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        ErrorHandler::handle(ErrorCode::UDP_RECEIVE_FAILURE);
    }
    // ... 其他处理代码
}

class Logger {
public:
    static void log(const std::string& message) {
        std::cerr << "[ERROR]: " << message << std::endl;
        // TODO: 你可以在这里添加更复杂的日志记录功能，例如写入文件或发送到远程服务器
    }
};

class ErrorHandler {
public:
    static void handle(ErrorCode code) {
        std::string errorMessage;

        switch (code) {
            case ErrorCode::SUCCESS:
                // No error
                break;
            case ErrorCode::UDP_SEND_FAILURE:
                errorMessage = "UDP send failed!";
                break;
            case ErrorCode::UDP_RECEIVE_FAILURE:
                errorMessage = "UDP receive failed!";
                break;
            // ... handle other errors
        }

        if (!errorMessage.empty()) {
            Logger::log(errorMessage);
            displayError(errorMessage);
        }
    }

private:
    static void displayError(const std::string& message) {
        // 用于显示错误消息到用户界面的代码。
        // 这里我们简单地打印到stderr，但你可以扩展到GUI提示或其他方法。
        std::cerr << message << std::endl;
    }
};

#include <glog/logging.h>

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    // ... 其他代码
    return 0;
}
class Logger {
public:
    static void log(const std::string& message) {
        LOG(ERROR) << message;
    }
};
FLAGS_log_dir = "/path/to/log/directory";

