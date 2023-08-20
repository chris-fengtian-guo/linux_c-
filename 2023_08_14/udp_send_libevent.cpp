#include <event2/event.h>
#include <event2/thread.h>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

struct UdpMessage {
    sockaddr_in destination;
    std::string data;
};

std::queue<UdpMessage> sendQueue;
std::mutex queueMutex;
std::condition_variable queueCondition;
bool running = true;

void udp_send_thread(int udpSocket) {
    while (running) {
        UdpMessage message;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, [] { return !sendQueue.empty() || !running; });

            if (!running && sendQueue.empty()) break;
            message = sendQueue.front();
            sendQueue.pop();
        }

        sendto(udpSocket, message.data.c_str(), message.data.size(), 0,
               (struct sockaddr *)&message.destination, sizeof(message.destination));
    }
}
void enqueue_message(const sockaddr_in& destination, const std::string& data) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        sendQueue.push({destination, data});
    }
    queueCondition.notify_one();
}
int main() {
    // 初始化libevent的线程支持
    evthread_use_pthreads();

    // 创建UDP套接字
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        Logger::log("Failed to create UDP socket");
        return 1;
    }

    // 设置套接字为非阻塞模式
    evutil_make_socket_nonblocking(udpSocket);

    // 启动UDP发送线程
    std::thread sender(udp_send_thread, udpSocket);

    // ... 其他事件或处理

    running = false;
    queueCondition.notify_one();
    sender.join();

    close(udpSocket);
    return 0;
}
