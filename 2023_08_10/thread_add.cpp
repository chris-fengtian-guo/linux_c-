void threadFunction() {
    // ... 你的线程函数代码 ...

    // 查找并移除当前线程
    mtx.lock();
    for (auto it = threads.begin(); it != threads.end(); ++it) {
        if (it->thread.get_id() == std::this_thread::get_id()) {
            it->status.isRunning = false;  // 设置状态为不运行
            if (it->thread.joinable()) {
                it->thread.detach();  // 分离线程，使其不再joinable
            }
            threads.erase(it);  // 从容器中移除
            break;
        }
    }
    mtx.unlock();
}

#include <iostream>
#include <thread>
#include <chrono>
 
void foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
 
int main()
{
    std::thread t;
    std::cout << "before starting, joinable: " << std::boolalpha << t.joinable()
              << '\n';
 
    t = std::thread(foo);
    std::cout << "after starting, joinable: " << t.joinable() 
              << '\n';
 
    t.join();
    std::cout << "after joining, joinable: " << t.joinable() 
              << '\n';
}
