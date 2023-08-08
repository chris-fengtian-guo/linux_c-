#include <iostream>
#include <thread>
#include <pthread.h>
#include <chrono>

void threadFunction() {
    while (true) {
        std::cout << "Thread is working..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread myThread(threadFunction);

    std::this_thread::sleep_for(std::chrono::seconds(5)); // 让线程运行5秒

    // 获取std::thread的原生句柄，并尝试取消线程
    pthread_t nativeHandle = myThread.native_handle();
    pthread_cancel(nativeHandle);

    // 等待线程终止
    myThread.join();

    return 0;
}
