#include <iostream>
#include <string>

// 1. 基本函数模板
template <typename T> T myMax(T a, T b) { return (a > b) ? a : b; }

// 2. 多参数函数模板
template <typename T, typename U> void printPair(T a, U b) { std::cout << "Pair: " << a << ", " << b << std::endl; }

// 3. 默认模板参数
template <typename T = int> void printType(T value) {
    std::cout << "Type: " << typeid(T).name() << ", Value: " << value << std::endl;
}

int main() {
    // 使用基本函数模板
    std::cout << "Max of 3 and 7: " << myMax(3, 7) << std::endl;
    std::cout << "Max of 3.14 and 2.72: " << myMax(3.14, 2.72) << std::endl;

    // 使用多参数函数模板
    printPair(42, "answer");
    printPair("hello", 3.14);

    // 使用默认模板参数
    printType(10);           // 使用默认 int 类型
    printType<double>(20.5); // 明确指定 double 类型

    return 0;
}