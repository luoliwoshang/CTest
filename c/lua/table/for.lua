local person = {
    name = "John",
    age = 30
}
print(person.name)  -- 输出 "John"
print(person["age"])  -- 输出 30
person.job = "Programmer"
person["salary"] = 50000
print("--  ---")
for key, value in pairs(person) do
    print(key, value)
end
print("-- array ---")
for index, value in ipairs(fruits) do
    print(index, value)
end


