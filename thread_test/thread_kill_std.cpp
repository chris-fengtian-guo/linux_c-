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

    std::this_thread::sleep_for(std::chrono::seconds(5)); 

    
    pthread_t nativeHandle = myThread.native_handle();
    pthread_cancel(nativeHandle);

    
    myThread.join();

    return 0;
}
