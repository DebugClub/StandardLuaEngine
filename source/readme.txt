优化：
·Lua调用C函数传参数的时候，要new对象，可以做对象池
·Lua调用C函数传参数的时候，参数可以用upvalue或局部变量表示，节省寻址时间
·map可以用trie数替代

