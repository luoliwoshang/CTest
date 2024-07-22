#ifndef TEST_H
#define TEST_H

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

class OutTest
{
public:
    void sayHello();
    int add(int a, int b);
    void printMessage(const char *message);
};

void sayHello();

#endif // TEST_H
