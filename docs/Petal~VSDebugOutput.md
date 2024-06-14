# Petal~VSDebugOutput

## 概述

命名空间 Petal  

对 `OutputDebugStringA/W` 进行封装，并提供用于 Debug 输出的 `print` `println` 函数。  

### 向 VS 输出窗口进行输出

#### dout 和 dowt 对象

`VSDebugOutput` 是一个接口类模板，接口有二：  

1. `Write` 参数为 `std::basic_string_view<CharType>`， 用于输出字符串；  
2. `WriteCStr` 参数为 `const CharType*`，用于输出 C 风格字符串(需要用户保证 null-terminated)。  

`VSDebugOutputA` 和 `VSDebugOutputW` 封装对 VS 输出窗口进行 Debug 输出的函数 `OutputDebugStringA/W`，派生自 `VSDebugOutput<CharType>`.  
对 `VSDebugOutputA/W` 的基类 `Petal::Abstract::Output` 有重载的 `operator<<` 使用 `Write` 接口进行输出。  

`dout` 是持有 `VSDebugOutputA` 类型的对象。  
`dowt` 是持有 `VSDebugOutputW` 类型的对象。  

示例：  

```C++
#include "Petal~VSDebugOutput.h"
void foo(std::string s, std::string_view sv)
{
    using Petal::dout; // VSDebugOutputA 的对象
    using Petal::ln; // 输出换行的函数模板

    dout.Write("Hello ");
    dout.WriteCStr("world"); // C 风格字符串
    ln(dout);

    dout << "Hello world" << ln;


    dout.Write(s);
    dout.Write(sv);
    dout << ln;

    dout.WriteCStr(s.c_str());
    dout.WriteCStr(std::string{ sv }.c_str());
    // string_view 不保证 null-terminated，
    // 因此需要构造一个 std::string，
    // 它保证 null-terminated 并给出了 c_str 接口。
    dout << ln;
}
```

因为最终是和仅支持 C 风格字符串的 API `OutputDebugStringA/W` 打交道，故需要注意以下这些情况：  

```C++
#include "Petal~VSDebugOutput.h"
void foo()
{
    using Petal::dout; // VSDebugOutputA 的对象
    using Petal::ln; // 输出换行的函数模板

    // 构造一个表示 "abc\0def" 的字符串
    std::string s;
    s += "abc";
    s.push_back('\0');
    s += "def";

    dout.WirteCStr(s.c_str());
    // 输出结果为 "abc" (不带引号)
    // 期待的参数应为 C 风格字符串，由于 s 中存在空字符，
    // 输出内容被空字符切断了，因此仅输出了 "abc"
    ln(dout);

    dout.Write(s);
    // 按接口的意愿，效果应为输出 "abcdef" (不带引号，下同)('\0'不可见)
    // 实际输出结果为 "abcdef" 或 "abc"
    ln(dout);

    char non_null_term[3] = { 's', 'r', 'r' }; // 不以空结尾的"字符串"
    
    dout.Write(non_null_term);
    // 可能得到错误的输出，错误发生在构造 std::string_view 实参时。
    // 原因为 Write 接口的形参为 string_view 类型，
    // 它在初始化时将 non_null_term 视为了 C 风格字符串并进行求长度，
    // 最后可能得到了错误的结果。
    ln(dout);

    dout.WriteCStr(non_null_term);
    // 可能得到错误的输出
    ln(dout);

    dout.Write(std::string_view{ non_null_term, 3 });
    // 正确的输出
    // 虽然 string_view 不保证 null-terminated，
    // 但是 Write 允许参数为非 null-terminated 的 string_view，
    // 通过 string_view 的 size 可以获得要输出的范围，
    // 通过构造一个待输出字符串的副本并使副本保证 null-terminated 就可以进行正确的输出。
    ln(dout);
}
```

当预处理器定义宏 [`Petal_Enable_ForceDbgRemoveNUL`](./Preprocessor.md#Petal_Enable_ForceDbgRemoveNUL) 时，`dout/dowt` 及同类型对象输出前会剔除待输出文本的副本的空字符以保证正确的输出效果。否则它在输出字符串时将字符串解释为 C 风格字符串，输出效果则和 `WriteCStr` 接口一致。  

建议始终定义这个宏，以保证正确的输出效果。  

文档其余部分暂未完成！

## 参考
