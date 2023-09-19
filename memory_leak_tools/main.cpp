#include <iostream>

void func(std::string input)
{
	std::cout << "input" << input << "\n";
}
int main() {
	func("hello world");
    int *array = new int[100];
    array[120] = 0;  // 越界
    delete[] array;
    return 0;
}
