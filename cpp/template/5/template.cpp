#include <iostream>
#include <vector>

// 一个可能导致歧义的类
template <typename T> struct AmbiguousType {
    static int iterator; // 静态成员变量
    typedef T *pointer;  // 类型别名
};

template <typename T> int AmbiguousType<T>::iterator = 42;

template <typename T> class MyClass {
  public:
    // 正确：使用 typename
    typedef typename std::vector<T>::iterator CorrectIterator;

    // 错误：不使用 typename
    // typedef std::vector<T>::iterator WrongIterator;

    // 可能导致歧义的情况
    // typedef AmbiguousType<T>::iterator AmbiguousIterator;

    void example() {
        // 正确：使用 typename
        typename AmbiguousType<T>::pointer p = nullptr;

        // 错误：不使用 typename
        // AmbiguousType<T>::pointer q = nullptr;

        // 访问静态成员不需要 typename
        int value = AmbiguousType<T>::iterator;
    }
};

int main() {
    MyClass<int> obj;
    return 0;
}