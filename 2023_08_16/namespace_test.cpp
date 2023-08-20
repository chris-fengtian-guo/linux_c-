#include <iostream>

// 定义命名空间A
namespace A {
    int x = 1;
    void print() {
        std::cout << "A::x = " << x << std::endl;
    }
}

// 定义命名空间B
namespace B {
    int x = 2;
    void print() {
        std::cout << "B::x = " << x << std::endl;
    }
}

int main() {
    // 使用命名空间A中的标识符
    std::cout << "A::x = " << A::x << std::endl;
    A::print();

    // 使用命名空间B中的标识符
    std::cout << "B::x = " << B::x << std::endl;
    B::print();

    return 0;
}
