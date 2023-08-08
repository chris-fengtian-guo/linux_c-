#include <iostream>
#include <thread>
#include <csignal>
#include <chrono>
#include <cstdlib>


void signal_handler(int signal) {
    std::cout << "Received signal: " << signal << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::exit(signal);
}


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

    
    signal(SIGSEGV, signal_handler);
    signal(SIGFPE, signal_handler);

    switch (exit_code) {
        case 0:
            
            {
                std::thread thread(normal_thread_function);
                thread.join();
            }
            break;
        case 1:
            
            {
                std::thread thread([]() {
                    
                    int result = 1 / 0;
                    (void)result; 
                });
                thread.join();
            }
            break;
        case 2:
            
            {
                std::thread thread([]() {
                    
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
