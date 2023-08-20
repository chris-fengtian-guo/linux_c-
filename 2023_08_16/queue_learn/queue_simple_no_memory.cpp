#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <typename T>
class ThreadSafeQueue {
public:
    void enqueue(T item) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(item);
        m_condition.notify_one();
    }

    bool dequeue(T& item) {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty()) {
            m_condition.wait(lock);
        }
        item = m_queue.front();
        m_queue.pop();
        return true;
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

struct MyData {
    int id;
    std::string message;
};

int main() {
    ThreadSafeQueue<MyData> myDataQueue;

    // Enqueue data
    MyData data1 = {1, "hello"};
    myDataQueue.enqueue(data1);

    MyData data2 = {2, "world"};
    myDataQueue.enqueue(data2);

    // Dequeue data
    MyData result;
    while (!myDataQueue.empty()) {
        myDataQueue.dequeue(result);
        std::cout << "ID: " << result.id << ", Message: " << result.message << std::endl;
    }

    return 0;
}
