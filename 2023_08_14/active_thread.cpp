#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

const int MAX_CONCURRENT_THREADS = 5;

std::queue<std::function<void()>> task_queue; // 任务队列
std::vector<std::thread> active_threads; // 活跃线程列表
std::mutex mtx;
std::condition_variable cv_task_available;

void thread_worker() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv_task_available.wait(lock, [] { return !task_queue.empty(); });
            task = task_queue.front();
            task_queue.pop();
        }

        task(); // 执行任务

        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = std::find_if(active_threads.begin(), active_threads.end(),
                                   [](std::thread &t) { return t.get_id() == std::this_thread::get_id(); });

            if (it != active_threads.end()) {
                it->detach();
                active_threads.erase(it);
            }
        }
    }
}

void add_task(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(mtx);
    if (active_threads.size() < MAX_CONCURRENT_THREADS) {
        active_threads.emplace_back(thread_worker);
        cv_task_available.notify_one();
    }
    task_queue.push(task);
}

int main() {
    // 测试任务
    for (int i = 0; i < 10; ++i) {
        add_task([i] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " completed by thread " << std::this_thread::get_id() << std::endl;
        });
    }

    // 等待所有线程完成
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(mtx);
        if (active_threads.empty() && task_queue.empty()) {
            break;
        }
    }

    return 0;
}
