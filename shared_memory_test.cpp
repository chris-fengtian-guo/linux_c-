#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <algorithm>
#include <iostream>

int main()
{
	using namespace boost::interprocess;

	shared_memory_object shdmem{open_or_create, "Boost2", read_write};
	shdmem.truncate(2048);
	mapped_region region{shdmem, read_write};

	int *mem = static_cast<int *>(region.get_address());
	std::size_t tsize = region.get_size();
	std::size_t size = tsize/sizeof(int);
	mem[0] = 0x1234;

	shared_memory_object::remove("Boost2");
}
