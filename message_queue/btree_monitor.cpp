#include <iostream>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>

enum BT_STATUS {
    NOT_RUNNING,
    RUNNING,
    FINISHED
};

struct BtTaskStatus {
    unsigned int task_uuid;
    std::thread::id thread_id;
    BT_STATUS status;
    unsigned int bt_id;
};

class BtTaskManager {
public:
    BtTaskManager() : next_task_uuid_(0) {}

    unsigned int addTask(unsigned int bt_id, std::function<void()> task_func) {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        BtTaskStatus status;
        status.task_uuid = next_task_uuid_;
        status.bt_id = bt_id;
        status.status = RUNNING;
        status.thread_id = std::thread(task_func).get_id();
        tasks_[next_task_uuid_] = status;
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
    std::mutex tasks_mutex_;
};

void btree_execute(BtTaskManager &manager, unsigned int bt_id) {
    // Here is where you would add the actual execution of the behavior tree
    std::cout << "Executing behavior tree with id: " << bt_id << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    BtTaskManager manager;

    // Add a new task
    unsigned int task_uuid = manager.addTask(1, [&]() { btree_execute(manager, 1); });

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
