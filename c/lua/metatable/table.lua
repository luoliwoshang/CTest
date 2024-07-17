local set1 = {10, 20, 30}
local set2 = {1, 2, 3}

-- 定义一个元表
local mt = {
    __add = function (table1, table2)  -- 定义两个表相加的行为
        local sumSet = {}
        for i = 1, #table1 do
            sumSet[i] = table1[i] + table2[i]
        end
        return sumSet
    end
}

setmetatable(set1, mt)  -- 设置元表
local sumSet = set1 + set2  -- 使用元表中定义的加法
for i, v in ipairs(sumSet) do
    print(v)  -- 输出 11, 22, 33
end
