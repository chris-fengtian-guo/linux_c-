#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct Message {
    std::vector<uint8_t> data;
    sockaddr_in target_addr;
};
class MessageQueue {
public:
    void push(const Message& msg) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(msg);
        cond_var_.notify_one();
    }

    void run() {
        std::thread sender_thread(&MessageQueue::send_loop, this);
        sender_thread.detach();
    }

private:
    void send_loop() {
        int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_fd < 0) {
            perror("Socket creation failed");
            return;
        }

        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_var_.wait(lock, [this] { return !queue_.empty(); });

            Message msg = queue_.front();
            queue_.pop();

            sendto(socket_fd, msg.data.data(), msg.data.size(), 0, (const sockaddr*)&msg.target_addr, sizeof(msg.target_addr));
        }

        close(socket_fd);
    }

    std::queue<Message> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};


int main() {
    MessageQueue message_queue;
    message_queue.run();

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &target_addr.sin_addr);

    Message msg;
    msg.data = {72, 101, 108, 108, 111, 44, 32, 85, 68, 80, 33}; // "Hello, UDP!"
    msg.target_addr = target_addr;

    message_queue.push(msg);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
