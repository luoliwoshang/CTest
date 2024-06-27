function printNumbers()
    for i = 1, 5 do
        print(i)
        coroutine.yield()  -- 挂起协同程序
    end
end

co = coroutine.create(printNumbers)  -- 创建协同程序
coroutine.resume(co)  -- 输出 1
coroutine.resume(co)  -- 输出 2
coroutine.resume(co)  -- 输出 3
coroutine.resume(co)  -- 输出 4
coroutine.resume(co)  -- 输出 5
