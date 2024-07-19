#ifndef TEST_H
#define TEST_H

void sayHello();

class OutTest
{
public:
    void sayHello();
    int add(int a, int b);
    void printMessage(const char *message);
};

namespace MyNamespace
{

    class Test
    {
    public:
        void sayHello();
        int add(int a, int b);
        void printMessage(const char *message);
    };

} // namespace MyNamespace

// 另外的命名空间
namespace AnotherNamespace
{

    class AnotherTest
    {
    public:
        void sayHello();
        int add(int a, int b);
        void printMessage(const char *message);
    };

} // namespace AnotherNamespace

#endif // TEST_H
