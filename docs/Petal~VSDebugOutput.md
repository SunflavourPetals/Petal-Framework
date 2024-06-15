# Petal~VSDebugOutput

## 概述

命名空间 Petal  

对 WIN32 API `OutputDebugStringA/W` 进行封装，并提供用于 Debug 输出的 `print` `println` 函数。  

### 向 VS 输出窗口进行输出

#### dout 和 dowt 对象

`VSDebugOutput` 是一个接口类模板，派生自 `Abstract::Output`，接口有二：  

1. `Write` 参数为 `std::basic_string_view<CharType>`， 用于输出字符串，继承自 `Output`；  
2. `WriteCStr` 参数为 `const CharType*`，用于输出 C 风格字符串(需要用户保证 null-terminated)。  

`VSDebugOutputA` 和 `VSDebugOutputW` 封装对 VS 输出窗口进行 Debug 输出的函数 `OutputDebugStringA/W`，派生自 `VSDebugOutput<Char/WChar>`.  
对 `VSDebugOutputA/W` 的基类 `Abstract::Output` 有重载的 `operator<<` 使用 `Write` 接口进行输出，但是只有关于输出字符串功能的重载。  

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

    // dout << 1; // 没有这个重载
    dout << std::format("{}", 1) << ln;

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
    // 输出内容被空字符切断了，因此仅输出了 "abc"。
    ln(dout);

    dout.Write(s);
    // 按接口的意愿，输出效果应为 "abcdef" (不带引号，下同)('\0'不可见)
    // 实际输出结果为 "abcdef" 或 "abc"，
    // 原因见下方有关 Petal_Enable_ForceDbgRemoveNUL 的说明。
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
    // 对于实现，通过构造一个待输出字符串的副本并使副本保证 null-terminated 就可以进行正确的输出。
    ln(dout);
}
```

当预处理器定义宏 [`Petal_Enable_ForceDbgRemoveNUL`](./Preprocessor.md#Petal_Enable_ForceDbgRemoveNUL) 时，`dout/dowt` 及同类型对象输出前会剔除待输出文本的副本的空字符以保证正确的输出效果。否则它在输出字符串时将字符串解释为 C 风格字符串，输出效果则和 `WriteCStr` 接口一致。  

建议始终定义这个宏，以保证正确的输出效果。  

#### print 和 println 函数模板

可见 `Output` 及 `VSDebugOutput` 这些接口模板都没有输出字符串外的输出接口，为了输出整数、浮点数等数据，我们需要将其转化为字符串再进行输出。`Petal::Debug` 和 `Petal::Debug::V` 中有 `print` `println` `wprint` 和 `wprintln` 模板，使用 C++20 `format` 库进行格式化，这样就方便我们输出字符串外的内容了。  

`Petal::Debug` 中的 print 系列函数使用 `basic_format_string<CharType>` 作为格式参数，需要格式字符串为编译时常量，`Petal::Debug::V` 中的 print 系列函数使用 `basic_string_view<CharType>` 作为格式参数。  

示例：  

```C++
#include "Petal~VSDebugOutput.h"
void foo()
{
    using namespace Petal::Debug;
    
    print("Hello{}", ' ');
    print("world{}", '!');
    println();

    println("Hello world!");

    println("{} {} {}", 1, 2, 3);

    std::string fmt = "{} {} {}";

    V::println(fmt, 1, 2, 3);
}
```

输出：  

```output
Hello world!
Hello world!
1 2 3
1 2 3
```

#### Petal_VSDebugOutput 系列宏

使用 `Petal_VSDebugOutput` 系列宏进行 Debug 输出。  

当预处理器定义宏 [`Petal_Enable_VSDebugOutput`](./Preprocessor.md#petal_enable_vsdebugoutput) 后，`Petal_VSDebugOutput` 系列宏将对括号内的内容进行输出，否则该系列宏被定义为空，不会生成任何代码。  

`Petal_VSDebugOutput` 系列宏使用 `dout` `dowt` 对象的 `WriteCStr` 接口进行输出，因此只接受 C 风格字符串。  

在定义宏 [`Petal_Enable_VSDebugOutput`](./Preprocessor.md#petal_enable_vsdebugoutput) 情况下，`Petal_VSDebugOutput` 系列宏的行为如下：  

1. `Petal_VSDebugOutputA` 使用 `dowt` 进行输出；  
2. `Petal_VSDebugOutputW` 使用 `dout` 进行输出；  
3. `Petal_VSDebugOutput`  根据 [`TChar`](./Petal~String.md#类型别名-tchar) 的实际类型选择 `dout` 或 `dowt` 进行输出；  
4. `Petal_VSDebugOutputT` 将字符串字面量制作为 `const TChar[N]` 类型的字面量进行输出；  
5. `Petal_VSDebugOutput_S` 根据 WIN32 `TCHAR` 选择 `dout` 或 `dowt` 进行输出；  
6. `Petal_VSDebugOutput_ST` 将字符串字面量制作为 WIN32 `const TCHAR[N]` 类型的字面量进行输出。  

示例：  

```C++
#include "Petal~VSDebugOutput.h"
using Petal::TChar;
void foo(const TChar* tstr, LPCTSTR win32_tstr)
{
    // 输出 const char* 类型的 C 风格字符串
    Petal_VSDebugOutputA("output a\r\n");

    // 输出 const wchar_t* 类型的 C 风格字符串
    Petal_VSDebugOutputW(L"output w\r\n");

    // 输出 const Petal::TChar* 类型的 C 风格字符串
    Petal_VSDebugOutput(tstr);
#ifdef Petal_Enable_Unicode
    Petal_VSDebugOutput(L"output Unicode ver.\r\n");
#else
    Petal_VSDebugOutput("output Multibyte ver.\r\n");
#endif

    // 输出 const Petal::TChar* 类型的 C 风格字符串
    // 用户只需填入双引号和其围住的待输出文本，是否添加 'L' 由该宏确定和完成，
    // 因此无论怎样，我们始终只需填入仅双引号包围的文本 "..." 而不是 L"..."
    // 该宏可能依赖了 msvc 的拓展
    Petal_VSDebugOutputT("output text\r\n");

    // 输出 const TCHAR* 类型的 C 风格字符串
    Petal_VSDebugOutput_S(win32_tstr);
#ifdef UNICODE
    Petal_VSDebugOutput_S(L"output s UNICODE ver.\r\n");
#else
    Petal_VSDebugOutput_S("output s Multibyte ver.\r\n");
#endif

    // 输出 const TCHAR* 类型的 C 风格字符串
    // 实现和使用和宏 Petal_VSDebugOutputT 类似
    Petal_VSDebugOutput_ST("output s text\r\n");
}
```

#### Petal_VSDbg 系列宏

本框架一些组件在被调用时可能产生 Debug 输出，并且输出的字符都在 ASCII 范围内，使用 `WCHAR` 版本的字符串将导致更多的空间被浪费，[`DbgChar`](./Petal~String.md#类型别名-dbgchar) 将根据 [`Petal_Enable_ForceDbgANSI`](./Preprocessor.md#petal_enable_forcedbgansi) 宏的定义情况被定义为 `char` | `TChar`，这样可以通过定义此宏减少空间的浪费，因此框架内的输出都是使用 `Petal_VSDbg` 和 `Petal_VSDbgT` 完成的。  

1. `Petal_VSDbgA` 与 `Petal_VSDebugOutputA` 行为一致  
2. `Petal_VSDbgW` 与 `Petal_VSDebugOutputW` 行为一致  
3. `Petal_VSDbg` 根据 [`DbgChar`](./Petal~String.md#类型别名-dbgchar) 选择 `dout` 或 `dowt` 进行输出；  
4. `Petal_VSDbgT` 将字符串字面量制作为 `const DbgChar[N]` 类型的字面量进行输出；  

## 参考

文档其余部分暂未完成！
