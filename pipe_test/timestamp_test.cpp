#include <iostream>
#include <chrono>

int main() {
    
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Timestamp: " << timestamp << std::endl;

    return 0;
}
