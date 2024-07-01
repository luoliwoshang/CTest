local fruits ={}
table.insert(fruits, "mango")  -- 默认插入到数组末尾
table.insert(fruits, 2, "kiwi")  -- 在位置 2 插入新元素
print(table.remove(fruits))  -- 删除并返回数组的最后一个元素
print(table.remove(fruits, 1))  -- 删除并返回数组的第一个元素
