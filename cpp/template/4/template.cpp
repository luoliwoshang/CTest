#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T> class DynamicArray {
  private:
    std::vector<T> data;

  public:
    void add(const T &element) { data.push_back(element); }

    void print() const {
        // 使用 auto
        for (const auto &item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;

        // 不使用 auto
        for (typename std::vector<T>::const_iterator it = data.begin(); it != data.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    DynamicArray<int> intArray;
    intArray.add(1);
    intArray.add(2);
    intArray.add(3);

    std::cout << "Int Array: ";
    intArray.print();

    // 使用 auto 与复杂类型
    std::map<std::string, std::vector<int>> complexMap;
    complexMap["key"] = {1, 2, 3};

    // 使用 auto
    for (const auto &pair : complexMap) {
        std::cout << pair.first << ": ";
        for (const auto &value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    // 不使用 auto
    for (std::map<std::string, std::vector<int>>::const_iterator it = complexMap.begin(); it != complexMap.end();
         ++it) {
        std::cout << it->first << ": ";
        for (std::vector<int>::const_iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt) {
            std::cout << *vecIt << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}