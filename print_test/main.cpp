#include <typeinfo>
#include <iostream>

class MyClass {
public:
    void myMethod() {
        std::cout << "Class name: " << typeid(*this).name() << ", Function name: " << __func__ << std::endl;
    }
};

int main(void)
{
	MyClass myClass;
	myClass.myMethod();
}
