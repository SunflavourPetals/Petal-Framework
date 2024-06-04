# Petal~BasicTypes.h

## 概述

为框架中的常用类型定义了一系列别名。  

## 参考

### 宏

#### 宏 Petal_Header_BasicTypes

指示头文件引入情况。  

### 命名空间 Petal

#### 类型别名 achar

`using achar = char;`

#### 类型别名 boolean

`using boolean = bool;`  

和 c++ 的 bool 类型保持一致，而非 `typedef unsigned char boolean`。  

#### 类型别名 byte

`using byte = u8;`  

将 `byte` 定义为 `u8`，可以进行算数运算，与 c++ 的 `std::byte` 不一致。

#### 类型别名 f32

`using f32 = float;`

#### 类型别名 f64

`using f64 = double;`

#### 类型别名模板 fptr

`template <typename ReturnT, typename... Args> using fptr = ReturnT(*)(Args...);`  

表示一个指向 `ReturnT(Args...)` 类型函数的指针：  

```cpp
#include "Petal~BasicTypes.h"
#include "Petal~VSDebugOutput.h"
double foo(int number)
{
    Petal::Debug::println("{}", number);
    return number * 0.5;
}
void bar()
{
    Petal::fptr<double, int> foo_ptr{ &foo };
    Petal::Debug::println("{}", foo_ptr(5));
}
```

输出  

```output
5
2.5
```

#### 类型别名 i16

`using i16 = typename ::std::int16_t;`

#### 类型别名 i32

`using i32 = typename ::std::int32_t;`

#### 类型别名 i64

`using i64 = typename ::std::int64_t;`

#### 类型别名 i8

`using i8 = typename ::std::int8_t;`

#### 类型别名模板 ptr

`template <typename Ty> using ptr = Ty*;`  

表示一个指向 `Ty` 的指针：  

```cpp
#include "Petal~BasicTypes.h"
#include "Petal~VSDebugOutput.h"
void foo()
{
    int number{ 123 };
    Petal::ptr<int> int_ptr{ &number };
    *int_ptr = 321;
    Petal::Debug::println("{}", number);
}
```

输出 `321`。  

#### 类型别名模板 ptrc

`template <typename Ty> using ptrc = const Ty*;`  

表示一个指向 `const Ty` 的指针：  

```cpp
#include "Petal~BasicTypes.h"
#include "Petal~VSDebugOutput.h"
void foo()
{
    int number{ 123 };
    Petal::ptrc<int> cint_ptr{ &number }; // 允许将 &number 赋值给 const int* 类型的对象 cint_ptr
    // *cint_ptr = 321; // 无法通过解引用 cint_ptr 修改所指向的对象，表达式 *cint_ptr 持有类型 const int&，
    Petal::Debug::println("{}", *cint_ptr);
}
```

输出 `123`。  

#### 类型别名 tsize

`using tsize = typename ::std::size_t;`  

即 `std::size_t`。  

#### 类型别名 u16

`using u16 = typename ::std::uint16_t;`

#### 类型别名 u16char

`using u16char = char16_t;`

#### 类型别名 u32

`using u32 = typename ::std::uint32_t;`

#### 类型别名 u32char

`using u32char = char32_t;`

#### 类型别名 u64

`using u64 = typename ::std::uint64_t;`

#### 类型别名 u8

`using u8 = typename ::std::uint8_t;`

#### 类型别名 u8char

`using u8char = char8_t;`

#### 类型别名 wchar

`using wchar = wchar_t;`
