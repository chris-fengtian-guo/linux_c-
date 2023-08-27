#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

const size_t maxThreads = 5;
std::queue<std::thread*> threadQueue;
std::mutex mtx;
bool running = true;

void behavior_tree_task(int id) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << id << " completed by thread " << std::this_thread::get_id() << std::endl;
}

void producer() {
    int taskID = 0;
    while (running) {
        std::lock_guard<std::mutex> lock(mtx);
        if (threadQueue.size() < maxThreads) {
            std::thread* t = new std::thread(behavior_tree_task, taskID++);
            threadQueue.push(t);
            std::cout << "Created thread for task " << taskID - 1 << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // To space out thread creation
    }
}

void consumer() {
    while (running) {
        std::thread* currentThread = nullptr;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!threadQueue.empty()) {
                currentThread = threadQueue.front();
                threadQueue.pop();
            }
        }

        if (currentThread) {
            currentThread->join();
            delete currentThread;
        }
    }
}

int main() {
    // Start the producer and consumer threads
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    // For the purpose of this example, let them run for 15 seconds and then stop
    std::this_thread::sleep_for(std::chrono::seconds(15)); 
    running = false;
    
    producerThread.join();
    consumerThread.join();

    return 0;
}
