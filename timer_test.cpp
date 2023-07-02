#include <iostream>
#include <thread>

void foo()
{
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::thread t(foo);
    t.detach(); // Detach the thread

    return 0;
}
