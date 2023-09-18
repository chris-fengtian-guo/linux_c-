#include <iostream>

int main() {
    int *array = new int[100];
    array[100] = 0;  // 越界
    delete[] array;
    return 0;
}
