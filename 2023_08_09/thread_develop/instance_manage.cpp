#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

const int MAX_THREADS = 5;
const std::chrono::seconds THREAD_TIMEOUT(30);
//const std::chrono::hours THREAD_TIMEOUT(24);

struct ThreadInfo {
    std::thread thread;
    std::chrono::steady_clock::time_point startTime;
};

std::queue<std::string> messages; // 消息队列
std::mutex mtx;
std::condition_variable cv;
std::vector<ThreadInfo> threads;

void threadFunction() {
    while (true) {
        std::cout << "Thread is working..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void checkThreadTimeout() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
        auto now = std::chrono::steady_clock::now();
        
        mtx.lock();
        for (auto it = threads.begin(); it != threads.end();) {
            if (now - it->startTime > THREAD_TIMEOUT) {
                pthread_t nativeHandle = it->thread.native_handle();
                pthread_cancel(nativeHandle);
                it->thread.join();
                it = threads.erase(it);
            } else {
                ++it;
            }
        }
        mtx.unlock();
    }
}

void threadHandler() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !messages.empty() && threads.size() < MAX_THREADS; });
        
        // 获取消息并解析（这里只是示例，你可以根据需要进行解析）
        std::string msg = messages.front();
        messages.pop();
        
        // 启动线程
        threads.emplace_back(ThreadInfo{ std::thread(threadFunction), std::chrono::steady_clock::now() });
        lock.unlock();
    }
}

int main() {
    std::thread handler(threadHandler);
    std::thread checker(checkThreadTimeout);

    // 示例：将消息添加到队列以进行处理
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // 间隔2秒发送消息

        mtx.lock();
        messages.push("Message " + std::to_string(i));
        cv.notify_all();
        mtx.unlock();
    }

    handler.join();
    checker.join();

    return 0;
}
