#include <iostream>

// 基类
class Base
{
public:
    virtual void func()
    {
        std::cout << "[Base] Function called" << std::endl;
    }
};

// 派生类
class Derived : public Base
{
public:
    void func() override
    {
        std::cout << "[Derived] Function called" << std::endl;
    }
};

void printVtable(void *obj)
{
    // 假设vptr在对象的起始位置
    void **vptr = *(void ***)obj;
    std::cout << "Vtable address: " << vptr << std::endl;
    std::cout << "First function address: " << vptr[0] << std::endl;
}

int main()
{
    Base base;
    Derived derived;

    std::cout << "Base object vtable:" << std::endl;
    printVtable(&base);

    std::cout << "Derived object vtable:" << std::endl;
    printVtable(&derived);

    return 0;
}
