#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>

enum BT_STATUS {
    NOT_RUNNING,
    RUNNING,
    FINISHED,
    ERROR
};

struct BtTaskStatus {
    unsigned int task_uuid;
    unsigned int bt_id;
    std::thread::id thread_id;
    BT_STATUS status;
};

class BtTaskManager {
public:
    BtTaskManager() : next_task_uuid_(0) {}
    unsigned int addTask(unsigned int bt_id, std::function<void()> task_func) {
    BtTaskStatus status;
    status.task_uuid = next_task_uuid_;
    status.bt_id = bt_id;
    status.status = RUNNING;
    {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        tasks_[next_task_uuid_] = status;
    }
    unsigned int task_uuid = status.task_uuid;
    std::thread task_thread([this, task_func, task_uuid]() mutable {
        task_func();
        updateTaskStatus(task_uuid, FINISHED);
    });
    {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        tasks_[task_uuid].thread_id = task_thread.get_id();
    }
    task_thread.detach();
    return next_task_uuid_++;
    }
       
    BtTaskStatus getTaskStatus(unsigned int task_uuid) {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        if(tasks_.find(task_uuid) != tasks_.end()) {
            return tasks_[task_uuid];
        }
        throw std::invalid_argument("Invalid task uuid");
    }

    void removeTask(unsigned int task_uuid) {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        if(tasks_.find(task_uuid) != tasks_.end()) {
            tasks_.erase(task_uuid);
            threads_.erase(task_uuid);
        } else {
            throw std::invalid_argument("Invalid task uuid");
        }
    }

    void updateTaskStatus(unsigned int task_uuid, BT_STATUS new_status) {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        if(tasks_.find(task_uuid) != tasks_.end()) {
            tasks_[task_uuid].status = new_status;
        } else {
            throw std::invalid_argument("Invalid task uuid");
        }
    }
private:
    unsigned int next_task_uuid_;
    std::map<unsigned int, BtTaskStatus> tasks_;
    std::map<unsigned int, std::thread> threads_;
    std::mutex tasks_mutex_;
    std::condition_variable task_added_cv_;
};

void btree_execute(BtTaskManager &manager, unsigned int task_uuid, unsigned int bt_id) {
    try {
        // Here is where you would add the actual execution of the behavior tree
        std::cout << "Executing behavior tree with id: " << bt_id << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        manager.updateTaskStatus(task_uuid, FINISHED);
    } catch (...) {
        // If an error occurs, update the status to ERROR
        manager.updateTaskStatus(task_uuid, ERROR);
    }
}

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

BtTaskManager manager;

int btree_monitor_process(Message &msg) {
    BtTaskManager manager;
    unsigned int req_id = msg.req_id;
    unsigned int bt_id = 10;
    // Add a new task
    unsigned int task_uuid = manager.addTask(req_id, [&]() { btree_execute(manager, req_id, bt_id); });

    // Get task status
    BtTaskStatus status = manager.getTaskStatus(task_uuid);
    std::cout << "Task " << status.task_uuid << " with bt_id " << status.bt_id << " is currently "
              << (status.status == RUNNING ? "running" : "not running") << std::endl;

    // Let's update task status and then remove it
    manager.updateTaskStatus(task_uuid, FINISHED);
    status = manager.getTaskStatus(task_uuid);
    std::cout << "Task " << status.task_uuid << " with bt_id " << status.bt_id << " is currently "
              << (status.status == FINISHED ? "finished" : "not finished") << std::endl;

    manager.removeTask(task_uuid);
    return 0;
}

int main() {
    MessageQueue queue;

    // Data management thread
    std::thread producer([&]() {
        for (unsigned int i = 0; i < 10; ++i) {
            Message message{i, "request_" + std::to_string(i)};
            std::cout << "producer send req_id: " << message.req_id << ", request: " << message.request << "\n";
            queue.push(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Behavior tree scheduling thread
    std::thread consumer([&]() {
        for (unsigned int i = 0; i < 10; ++i) {
            Message message = queue.pop();
            std::cout << "consumer req_id: " << message.req_id << ", request: " << message.request << "\n";
	    btree_monitor_process(message);
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
