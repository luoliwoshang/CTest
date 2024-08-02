#ifndef ZOO_H
#define ZOO_H

// 定义一些常量
#define MAX_ANIMALS 100
#define ZOO_NAME "Wildlife Wonder"
#define VERSION 1.2

// 定义一些状态码
#define OK 1
#define ERROR 0

// 定义一些动物类型
#define MAMMAL 1
#define BIRD 2
#define FISH 3

// 定义一些功能性宏
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define COMPUTE(x, y) (MAX(ABS(x), ABS(y)))

namespace forest {

class Bear {
  public:
    void roar();
    int eat(int berries, int fish);
    void sleep(const char *where);
};

} // namespace forest

namespace ocean {

class Shark {
  public:
    /**
    " * Calculates the sum of two integers.\n"
    " * @param a The first integer\n"
    " * @param b The second integer\n"
    " * @return The sum of a and b\n"
    " */
    void roar();
    int eat(int fish, int seals);
    void sleep(const char *where);
};

} // namespace ocean

class Eagle {
  public:
    void roar();
    int eat(int mice, int fish);
    void sleep(const char *where);
};

void zookeeper();

#endif // ZOO_H