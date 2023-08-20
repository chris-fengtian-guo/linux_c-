#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

struct Info {
    std::string uuid;
    std::string device_id;
    unsigned int count;
};

void threadFunction(const Info& data) {
    std::cout << "Thread started with UUID: " << data.uuid 
              << ", Device ID: " << data.device_id 
              << ", Count: " << data.count << std::endl;
    
    // 让线程运行，直到被外部取消
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    Info info1 = {"UUID_1", "DeviceID_1", 1};
    Info info2 = {"UUID_2", "DeviceID_2", 2};

    std::thread thread1(threadFunction, info1);
    std::thread thread2(threadFunction, info2);

    // 主线程等待30秒
    std::this_thread::sleep_for(std::chrono::seconds(30));

    // 尝试终止线程
    pthread_cancel(thread1.native_handle());
    pthread_cancel(thread2.native_handle());

    thread1.join();
    thread2.join();

    std::cout << "Threads terminated after 30 seconds." << std::endl;

    return 0;
}
