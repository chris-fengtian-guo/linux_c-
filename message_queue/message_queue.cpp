#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <string>

struct Message {
    unsigned int req_id;
    std::string request;
};

class MessageQueue {
public:
    void push(const Message& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(message);
        condition_.notify_one();
    }

    Message pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&]() { return !queue_.empty(); });
        Message message = queue_.front();
        queue_.pop();
        return message;
    }

private:
    std::queue<Message> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

int main() {
    MessageQueue queue;

    
    std::thread producer([&]() {
        for (unsigned int i = 0; i < 10; ++i) {
            Message message{i, "request_" + std::to_string(i)};
            std::cout << "producer send req_id: " << message.req_id << ", request: " << message.request << "\n";
            queue.push(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    
    std::thread consumer([&]() {
        for (unsigned int i = 0; i < 10; ++i) {
            Message message = queue.pop();
            std::cout << "consumer req_id: " << message.req_id << ", request: " << message.request << "\n";
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
