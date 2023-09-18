class MyClass {
#ifdef ENABLE_MYCLASS
public:
    MyClass() {}
#else
public:
    MyClass() = delete; // 或者把它设为 private
#endif
};

int main(void)
{
MyClass print;
print.MyMethod();
}
