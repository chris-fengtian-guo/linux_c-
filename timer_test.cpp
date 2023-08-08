#include <iostream>
#include <thread>

void foo()
{
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::thread t(foo);
    t.detach(); 

    return 0;
}
