#include <iostream>
#include <thread>
#include <csignal>
#include <chrono>
#include <cstdlib>

// 信号处理函数，用于模拟异常信号退出
void signal_handler(int signal) {
    std::cout << "Received signal: " << signal << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::exit(signal);
}

// 子线程函数，用于正常退出
void normal_thread_function() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Child thread completed normally" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <exit_code>" << std::endl;
        return 1;
    }

    int exit_code = std::atoi(argv[1]);
    std::cout << "input arg1=" << argv[1] << "\n";

    // 设置信号处理函数，用于模拟异常退出
    signal(SIGSEGV, signal_handler);
    signal(SIGFPE, signal_handler);

    switch (exit_code) {
        case 0:
            // 创建子线程并等待其退出
            {
                std::thread thread(normal_thread_function);
                thread.join();
            }
            break;
        case 1:
            // 创建子线程，模拟异常信号退出
            {
                std::thread thread([]() {
                    // 模拟除以零异常
                    int result = 1 / 0;
                    (void)result; // 防止编译器提示未使用变量
                });
                thread.join();
            }
            break;
        case 2:
            // 创建子线程，模拟除以零异常退出
            {
                std::thread thread([]() {
                    // 模拟访问无效内存
                    int* p = nullptr;
                    *p = 10;
                });
                thread.join();
            }
            break;
        default:
            std::cout << "Invalid exit code. Supported values: 0, 1, 2" << std::endl;
            return 1;
    }

    return exit_code;
}
