#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <optional>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() : _queue(), _mutex(), _cond_var() {}

    void enqueue(const T &item) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(item);
        _cond_var.notify_one();
    }
    T* dequeue() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty()) {
            _cond_var.wait(lock);
        }

        if (!_queue.empty()) {
            T* item = new T(_queue.front());
            _queue.pop();
            return item;
        }

        return nullptr;
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
    ThreadSafeQueue<MyData> queue1;
    ThreadSafeQueue<MyData> queue2;
    ThreadSafeQueue<MyData> queue3;

    // Populate the first queue
    queue1.enqueue(MyData{1, "Message 1"});
    queue1.enqueue(MyData{2, "Message 2"});
    queue1.enqueue(MyData{3, "Message 3"});
    queue1.enqueue(MyData{1, "Message 4"});
    queue1.enqueue(MyData{5, "Message 5"});

    // Process items in the first queue
    MyData* data;
    while (!queue1.isEmpty()) {
        data = queue1.dequeue();
        if (data != nullptr) {
            if (data->id == 1) {
                queue2.enqueue(*data);
            } else {
                queue3.enqueue(*data);
            }
            delete data; // Don't forget to free the memory!
        }
    }

    // Display contents of queue2
    std::cout << "Contents of queue2:" << std::endl;
    while (!queue2.isEmpty()) {
        data = queue2.dequeue();
        if (data != nullptr) {
            std::cout << "ID: " << data->id << ", Message: " << data->message << std::endl;
            delete data; // Don't forget to free the memory!
        }
    }

    // Display contents of queue3
    std::cout << "Contents of queue3:" << std::endl;
    while (!queue3.isEmpty()) {
        data = queue3.dequeue();
        if (data != nullptr) {
            std::cout << "ID: " << data->id << ", Message: " << data->message << std::endl;
            delete data; // Don't forget to free the memory!
        }
    }

    return 0;
}
