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

动态库编译，指定include，lib
```
gcc cjson_demo.c -o cjson_demo -I/opt/homebrew/Cellar/cjson/1.7.18/include -L/opt/homebrew/Cellar/cjson/1.7.18/lib -lcjson
```

静态库编译
```
gcc cjson_demo.c -o cjson_demo -I/opt/homebrew/Cellar/cjson/1.7.18/include /opt/homebrew/Cellar/cjson/1.7.18/lib/libcjson.a
```
可以使用 `otool -L cjson_demo` 验证动态库还是静态库
```
cjson_demo:
    /usr/local/lib/libcjson.dylib (compatibility version 1.0.0, current version 1.0.0)
    /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1213.0.0)

```