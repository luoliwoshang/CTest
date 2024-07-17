#include <iostream>

// 基类
class Base
{
public:
    // 构造函数
    Base()
    {
        std::cout << "[Base] Constructor called" << std::endl;
    }

    // 析构函数
    virtual ~Base()
    {
        std::cout << "[Base] Destructor called" << std::endl;
    }

    // 虚函数
    virtual void func()
    {
        std::cout << "[Base] Function called" << std::endl;
    }
};

// 派生类
class Derived : public Base
{
public:
    // 构造函数
    Derived()
    {
        std::cout << "[Derived] Constructor called" << std::endl;
    }

    // 析构函数
    ~Derived() override
    {
        std::cout << "[Derived] Destructor called" << std::endl;
    }

    // 重写虚函数
    void func() override
    {
        std::cout << "[Derived] Function called" << std::endl;
    }
};

// 调用函数
void callFunc(Base &b)
{
    b.func(); // 调用实际对象的函数实现
}

int main()
{
    std::cout << "Creating Base object:" << std::endl;
    Base base;
    std::cout << "Base object created" << std::endl;

    std::cout << "Creating Derived object:" << std::endl;
    Derived derived;
    std::cout << "Derived object created" << std::endl;

    std::cout << "Calling function on base object:" << std::endl;
    callFunc(base); // 输出 "[Base] Function called"

    std::cout << "Calling function on derived object:" << std::endl;
    callFunc(derived); // 输出 "[Derived] Function called"

    std::cout << "Base and Derived objects will be destroyed upon exiting main." << std::endl;

    return 0;
}
// Creating Base object:
// [Base] Constructor called
// Base object created
// Creating Derived object:
// [Derived] Constructor called
// Base object created
// Derived object created
// Calling function on base object:
// [Base] Function called
// Calling function on derived object:
// [Derived] Function called
// Base and Derived objects will be destroyed upon exiting main.
// [Derived] Destructor called
// [Base] Destructor called
// [Base] Destructor called

// 构造函数：在创建对象时自动调用。

// 当Base base对象被创建时，Base的构造函数被调用，输出[Base] Constructor called。
// 当Derived derived对象被创建时，首先调用Base的构造函数，然后调用Derived的构造函数，
// 分别输出[Base] Constructor called和[Derived] Constructor called。

// 析构函数：在对象被销毁时自动调用。

// 当main函数结束时，base和derived对象被销毁。
// 销毁derived对象时，首先调用Derived的析构函数，然后调用Base的析构函数，
// 分别输出[Derived] Destructor called和[Base] Destructor called。
