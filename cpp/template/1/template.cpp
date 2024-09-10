#include <iostream>
#include <string>

// 自定义命名空间
namespace mylib {
// 函数模板定义
template <typename T> T max(T a, T b) { return (a > b) ? a : b; }
} // namespace mylib

// 可以直接使用max的自定义类型
class Person {
  public:
    int age;
    Person(int a) : age(a) {}
    bool operator>(const Person &other) const { return age > other.age; }
};

// 不能直接使用max的自定义类型
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

int main() {
    // 基本类型
    std::cout << "Max of 3 and 7: " << mylib::max(3, 7) << std::endl;
    std::cout << "Max of 3.14 and 2.72: " << mylib::max(3.14, 2.72) << std::endl;

    // 可以使用的自定义类型
    Person p1(25), p2(30);
    Person older = mylib::max(p1, p2);
    std::cout << "Age of older person: " << older.age << std::endl;

    // 不能直接使用的自定义类型
    Point pt1(1, 2), pt2(3, 4);
    // 下面这行如果取消注释会导致编译错误
    // Point maxPoint = mylib::max(pt1, pt2); // 错误：没有为 'Point' 定义 '>' 运算符

    return 0;
}