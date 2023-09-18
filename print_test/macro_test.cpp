#ifdef ENABLE_PRINTING
class MyClass {
public:
    void MyMethod() {
        // 实际的打印操作
    }
};
#else
class MyClass {
public:
    void MyMethod() {
        // 什么也不做
    }
};
#endif

// 代码中
int main(void)
{
MyClass print;
print.MyMethod();
}
