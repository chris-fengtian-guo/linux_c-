#include <iostream>
#include <thread>
#include <stdexcept>

void threadFunction() {
    try {
        // 这里为了演示，我们主动抛出一个异常
        throw std::runtime_error("Some error occurred in the thread!");
        
        // 其他线程代码...
        
    } catch (const std::exception& e) {
        // 处理异常
        std::cerr << "Thread error: " << e.what() << std::endl;
    }
}

int main() {
    std::thread myThread(threadFunction);
    myThread.detach();

    std::cout << "Main thread continues..." << std::endl;
    
    // 这里只是为了演示，我们让主线程等待一段时间以确保子线程有机会运行
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
