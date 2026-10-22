print("Lua script loaded!")

a = 7 + 11 + math.sin(10)

-- Call the C++ function that you exposed with lua.set_function
result = cppMultiply(3, 4)
print("Result from C++ function: (FROM LUA)", result)
