#include <queue>
#include <mutex>
#include <memory>
#include <string>
#include <thread>

enum MessageType {
    FILE_TRANSFER,
    COMMAND_INFO,
    DEVICE_STATUS
};

struct Message {
    MessageType type;
    std::string uuid;
    std::string device_id;
    std::string content;  // 具体报文内容，可以根据需要进一步细化
};

class CommQueue {
private:
    std::queue<Message> queue_;
    std::mutex mtx_;

public:
    void push(const Message& msg) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(msg);
    }

    bool pop(Message& msg) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) {
            return false;
        }
        msg = queue_.front();
        queue_.pop();
        return true;
    }

    bool empty() const {
        return queue_.empty();
    }
};

class CommHandler {
private:
    CommQueue& queue_;
    int port_;
    MessageType type_;
    bool running_;
    std::thread thread_;

public:
    CommHandler(CommQueue& queue, int port, MessageType type)
        : queue_(queue), port_(port), type_(type), running_(false) {}

    void start() {
        running_ = true;
        thread_ = std::thread(&CommHandler::listen, this);
    }

    void stop() {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void listen() {
        while (running_) {
            // 使用对应的port_来接收数据
            // 假设数据已经被接收并存储在data变量中
            // std::string data = ...;

            // 根据type_解析data并生成对应的Message
            // Message msg = ...;

            // queue_.push(msg);
        }
    }

    ~CommHandler() {
        stop();
    }
};
int main() {
    CommQueue queue;
    CommHandler fileHandler(queue, 8001, FILE_TRANSFER);
    CommHandler cmdHandler(queue, 8002, COMMAND_INFO);
    CommHandler statusHandler(queue, 8003, DEVICE_STATUS);

    fileHandler.start();
    cmdHandler.start();
    statusHandler.start();

    // ... 其他代码

    fileHandler.stop();
    cmdHandler.stop();
    statusHandler.stop();

    return 0;
}
