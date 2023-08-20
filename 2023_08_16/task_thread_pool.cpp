#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

// 任务数据结构
struct Task {
    std::function<void(std::string)> callback;
    std::string message;
};

// 线程池类
class ThreadPool {
public:
    ThreadPool(size_t numThreads) : exit(false), threads(numThreads) {
        for (auto &t : threads) {
            t = std::thread([this] { this->worker(); });
        }
    }

    ~ThreadPool() {
        exit = true;
        cv.notify_all();
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
    void ThreadPool::shutdown() {
    exit = true;
    cv.notify_all();
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    }

    void enqueue(const Task &task) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.push(task);
        }
        cv.notify_one();
    }

private:
    void worker() {
        while (true) {
            Task task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return exit || !tasks.empty(); });

                if (exit && tasks.empty()) {
                    return;
                }

                task = tasks.front();
                tasks.pop();
            }
            task.callback();
        }
    }

    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool exit;
};

// 示例回调函数
void myCallback() {
    std::cout << "Task executed in thread " << std::this_thread::get_id() << std::endl;
}


int main() {
    ThreadPool pool(4); // 创建包含4个线程的线程池

    // 向线程池添加任务
    for (int i = 0; i < 10; ++i) {
        Task task = {myCallback, "Message " + std::to_string(i)};
        pool.enqueue(task);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待线程完成任务

    return 0;
}
