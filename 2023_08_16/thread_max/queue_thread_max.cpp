#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

const int MAX_THREADS = 5;
struct Task {
    int id;
    std::string data;
};
class Worker {
public:
    Worker(std::queue<Task>& tasks, std::mutex& mtx)
        : tasks_(tasks), mtx_(mtx) {}

    void operator()() {
        Task task;
        {
            std::unique_lock<std::mutex> lock(mtx_);
            if (!tasks_.empty()) {
                task = tasks_.front();
                tasks_.pop();
            } else {
                return;  // No tasks, exit the thread.
            }
        }
        processTask(task);
    }

private:
    void processTask(const Task& task) {
        std::cout << "Thread " << std::this_thread::get_id() << " processing task " << task.id << ": " << task.data << std::endl;
    }

    std::queue<Task>& tasks_;
    std::mutex& mtx_;
};
class ThreadPool {
public:
    ThreadPool(std::queue<Task>& tasks)
        : tasks_(tasks), mtx_(), threadCount_(0) {}

    void processTasks() {
        while (true) {
            {
                std::unique_lock<std::mutex> lock(mtx_);
                if (threadCount_ < MAX_THREADS && !tasks_.empty()) {
                    std::thread(Worker(tasks_, mtx_)).detach();
                    ++threadCount_;
                } else if (tasks_.empty() && threadCount_ == 0) {
                    break;  // No tasks and no threads, exit the loop.
                }
            }
        }
    }

private:
    std::queue<Task>& tasks_;
    std::mutex mtx_;
    int threadCount_;
};

int main() {
    std::queue<Task> tasks;
    for (int i = 0; i < 20; ++i) {
        Task task{i, "Task data " + std::to_string(i)};
        tasks.push(task);
    }

    ThreadPool threadPool(tasks);
    threadPool.processTasks();

    return 0;
}
