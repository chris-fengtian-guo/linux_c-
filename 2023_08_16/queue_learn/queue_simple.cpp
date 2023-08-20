#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() : _queue(), _mutex(), _cond_var() {}

    void enqueue(const T &item) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(item);
        _cond_var.notify_one();
    }

    bool dequeue(T &item) {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty()) {
            _cond_var.wait(lock);
        }

        if (!_queue.empty()) {
            item = _queue.front();
            _queue.pop();
            return true;
        }

        return false;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.size();
    }

    bool isEmpty() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.empty();
    }

private:
    std::queue<T> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _cond_var;
};

struct MyData {
    int id;
    std::string message;
};

int main() {
    ThreadSafeQueue<MyData> queue;

    MyData data1{1, "Hello"};
    MyData data2{2, "World"};

    queue.enqueue(data1);
    queue.enqueue(data2);

    MyData data;
    while (!queue.isEmpty()) {
        queue.dequeue(data);
        std::cout << "ID: " << data.id << ", Message: " << data.message << std::endl;
    }

    return 0;
}
