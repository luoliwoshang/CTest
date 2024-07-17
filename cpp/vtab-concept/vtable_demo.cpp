#include <iostream>

// 基类
class Base
{
public:
    virtual void func()
    {
        std::cout << "Base function" << std::endl;
    }
};

// 派生类
class Derived : public Base
{
public:
    void func() override
    {
        std::cout << "Derived function" << std::endl;
    }
};

// 打印vtable和调用第一个虚函数
void printVtable(void *obj)
{
    // 将 obj 解释为双重指针，并解引用获取 vptr
    void **vptr = *(void ***)obj;
    // vptr 是该对象的第一个成员，其地址为obj的地址，其值为vtable的地址

    std::cout << "Object address: " << obj << std::endl;  // Base:0x16b0aaca0 Derived:0x16b0aac98
    std::cout << "Vptr address: " << &vptr << std::endl;  // 均为同一个 Vptr address: 0x16b0aac50 ，为什么这里的 Vptr 的地址比 Object 地址小 80 字节
    std::cout << "Vtable address: " << vptr << std::endl; // Base: 0x1005cc0b0 Derived: 0x1005cc0d8

    // 将vptr转换为具体的函数指针类型指针
    using FuncPointer = void (*)(void);
    FuncPointer *funcPtr = (FuncPointer *)vptr;
    std::cout << "First function address in vtable: " << (void *)funcPtr[0] << std::endl;

    // 调用第一个虚函数
    funcPtr[0]();
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
