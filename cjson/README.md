## 预期
Name: John Doe
Age: 30
Generated JSON:
{
        "name": "John Doe",
        "age":  30,
        "has_license":  true
}
## 执行
-I 指定include 查找位置 -L指定Lib查找位置
-lcjson代表查找的是cjson的库
```
gcc cjson_demo.c -o cjson_demo -I/opt/homebrew/Cellar/cjson/1.7.18/include -L/opt/homebrew/Cellar/cjson/1.7.18/lib -lcjson
```