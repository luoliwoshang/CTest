#include <iostream>
#include <memory>
#include <vector>

// 一个简单的动态数组容器
template <typename T, typename Allocator = std::allocator<T>> class DynamicArray {
  private:
    std::vector<T, Allocator> data;

  public:
    void add(const T &element) { data.push_back(element); }

    void print() const {
        for (const auto &item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // 使用默认分配器
    DynamicArray<int> intArray;
    intArray.add(1);
    intArray.add(2);
    intArray.add(3);
    std::cout << "Int Array: ";
    intArray.print();

    // 使用自定义分配器
    DynamicArray<double, std::allocator<double>> doubleArray;
    doubleArray.add(1.1);
    doubleArray.add(2.2);
    doubleArray.add(3.3);
    std::cout << "Double Array: ";
    doubleArray.print();

    return 0;
}