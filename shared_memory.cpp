#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <algorithm> 
#include <iostream>

int main()
{
    using namespace boost::interprocess;

    shared_memory_object shdmem{open_or_create, "Boost", read_write};
    shdmem.truncate(1024);
    mapped_region region{shdmem, read_write};

    int *mem = static_cast<int*>(region.get_address());
    std::size_t size = region.get_size() / sizeof(int);

    for(std::size_t i = 0; i < size; ++i)
    {
        mem[i] = 1;
    }

    if(std::count_if(mem, mem + size, [](int x){ return x == 1; }) == static_cast<std::ptrdiff_t>(size))
    {
        std::cout << "The memory was correctly initialized\n";
    }
    else
    {
        std::cout << "The memory was not correctly initialized\n";
    }
    
    shared_memory_object::remove("Boost");

    return 0;
}
