# Petal~VSDebugOutput

## 概述

命名空间 Petal  

对 WIN32 API `OutputDebugStringA/W` 进行封装，并提供用于 Debug 输出的 `print` `println` 函数。  

### 向 VS 输出窗口进行输出

#### dout 和 dowt 对象

`VSDebugOutput` 是一个接口类模板，派生自 `Abstract::Output`，接口有二：  

1. `Write` 参数为 `std::basic_string_view<CharType>`， 用于输出字符串，继承自 `Output`；  
2. `WriteCStr` 参数为 `const CharType*`，用于输出 C 风格字符串(即意味着需要用户保证 null-terminated)。  

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

当预处理器定义宏 [`Petal_Enable_ForceDbgRemoveNUL`](./Preprocessor.md#Petal_Enable_ForceDbgRemoveNUL) 时，`dout/dowt` 及同类型对象输出前会剔除待输出文本的副本的空字符以保证正确的输出效果。否则它在输出字符串时将字符串解释为 C 风格字符串，输出效果则和 `WriteCStr(string(string_view_obj).c_str())` 接口一致。  

建议始终定义这个宏，以保证正确的输出效果。  

#### print 和 println 函数

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

当预处理器定义宏 [`Petal_Enable_VSDebugOutput`](./Preprocessor.md#petal_enable_vsdebugoutput) 后，`Petal_VSDebugOutput` 系列宏将对括号内的 C 风格字符串进行输出，否则该系列宏被定义为空，不会生成任何代码。  

`Petal_VSDebugOutput` 系列宏使用 `dout` `dowt` 对象的 `WriteCStr` 接口进行输出，因此只接受 C 风格字符串。  

在定义宏 [`Petal_Enable_VSDebugOutput`](./Preprocessor.md#petal_enable_vsdebugoutput) 情况下，`Petal_VSDebugOutput` 系列宏的行为如下：  

1. `Petal_VSDebugOutputA` 使用 `dowt` 进行输出；  
2. `Petal_VSDebugOutputW` 使用 `dout` 进行输出；  
3. `Petal_VSDebugOutput`  根据 [`TChar`](./Petal~String.md#类型别名-tchar) 的实际类型选择 `dout` 或 `dowt` 进行输出；  
4. `Petal_VSDebugOutputT` 将无前缀的字符串字面量制作为 `const TChar[N]` 类型的字面量进行输出；  
5. `Petal_VSDebugOutput_S` 根据 WIN32 `TCHAR` (准确来讲是宏 `UNICODE`)的类型选择 `dout` 或 `dowt` 进行输出；  
6. `Petal_VSDebugOutput_ST` 将无前缀的字符串字面量制作为 WIN32 `const TCHAR[N]` 类型的字面量进行输出。  

示例：  

```C++
#include "Petal~VSDebugOutput.h"
using Petal::TChar;
void foo(const TChar* tstr, LPCTSTR win32_tstr)
{
    // 输出 const char* 类型的 C 风格字符串
    Petal_VSDebugOutputA("output a\n");

    // 输出 const wchar_t* 类型的 C 风格字符串
    Petal_VSDebugOutputW(L"output w\n");

    // 输出 const Petal::TChar* 类型的 C 风格字符串
    Petal_VSDebugOutput(tstr);
#ifdef Petal_Enable_Unicode
    Petal_VSDebugOutput(L"output Unicode ver.\n");
#else
    Petal_VSDebugOutput("output Multibyte ver.\n");
#endif

    // 输出 const Petal::TChar* 类型的 C 风格字符串
    // 用户只能填入无前缀的字符串字面量，是否添加 'L' 由该宏确定和完成，
    // 该宏可能依赖了 msvc 的拓展
    Petal_VSDebugOutputT("output text\n");

    // 输出 const TCHAR* 类型的 C 风格字符串
    Petal_VSDebugOutput_S(win32_tstr);
#ifdef UNICODE
    Petal_VSDebugOutput_S(L"output s UNICODE ver.\n");
#else
    Petal_VSDebugOutput_S("output s Multibyte ver.\n");
#endif

    // 输出 const TCHAR* 类型的 C 风格字符串
    // 实现和使用和宏 Petal_VSDebugOutputT 类似
    Petal_VSDebugOutput_ST("output s text\n");
}
```

#### Petal_VSDbg 系列宏

本框架一些组件在被调用时可能产生 Debug 输出，并且输出的字符都在 ASCII 范围内，使用 `WCHAR` 版本的字符串将导致更多的空间被浪费，[`DbgChar`](./Petal~String.md#类型别名-dbgchar) 将根据 [`Petal_Enable_ForceDbgANSI`](./Preprocessor.md#petal_enable_forcedbgansi) 宏的定义情况被定义为 `char` | `TChar`，这样可以通过定义此宏减少空间的浪费，因此框架内的输出都是使用 `Petal_VSDbg` 和 `Petal_VSDbgT` 完成的。  

1. `Petal_VSDbgA` 与 `Petal_VSDebugOutputA` 行为一致  
2. `Petal_VSDbgW` 与 `Petal_VSDebugOutputW` 行为一致  
3. `Petal_VSDbg` 根据 [`DbgChar`](./Petal~String.md#类型别名-dbgchar) 选择 `dout` 或 `dowt` 进行输出；  
4. `Petal_VSDbgT` 将字符串字面量制作为 `const DbgChar[N]` 类型的字面量进行输出；  

## 参考

### 宏

#### 宏 Petal_Header_VSDebugOutput

指示头文件引入情况。  

#### 宏 Petal_VSDbg

定义：  

```C++
#if defined(Petal_Enable_ForceDbgANSI)
#define Petal_VSDbg(x) Petal_VSDebugOutputA(x)
#else
#define Petal_VSDbg(x) Petal_VSDebugOutput(x)
#endif
```

相关宏 [`Petal_Enable_ForceDbgANSI`](./Preprocessor.md#petal_enable_forcedbgansi)、[`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)、[`Petal_VSDebugOutput`](#宏-petal_vsdebugoutput)。

#### 宏 Petal_VSDbgA

定义：  

```C++
#define Petal_VSDbgA(x) Petal_VSDebugOutputA(x)
```

相关宏 [`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)。

#### 宏 Petal_VSDbgT

定义：  

```C++
#if defined(Petal_Enable_ForceDbgANSI)
#define Petal_VSDbgT(x) Petal_VSDebugOutputA(x)
#else
#define Petal_VSDbgT(x) Petal_VSDebugOutputT(x)
#endif
```

相关宏 [`Petal_Enable_ForceDbgANSI`](./Preprocessor.md#petal_enable_forcedbgansi)、[`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)、[`Petal_VSDebugOutputT`](#宏-petal_vsdebugoutputt)。

#### 宏 Petal_VSDbgW

定义：  

```C++
#define Petal_VSDbgW(x) Petal_VSDebugOutputW(x)
```

相关宏 [`Petal_VSDebugOutputW`](#宏-petal_vsdebugoutputw)。

#### 宏 Petal_VSDebugOutput

定义：  

```C++
#if defined(Petal_Enable_Unicode)
#define Petal_VSDebugOutput(x) Petal_VSDebugOutputW(x)
#else
#define Petal_VSDebugOutput(x) Petal_VSDebugOutputA(x)
#endif
```

相关宏 [`Petal_Enable_Unicode`](./Preprocessor.md#petal_enable_unicode)、[`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)、[`Petal_VSDebugOutputW`](#宏-petal_vsdebugoutputw)。

#### 宏 Petal_VSDebugOutputA

定义：  

```C++
#define Petal_VSDebugOutputA(x) ::Petal::dout.WriteCStr( x )
```

相关对象 [`dout`](#命名空间局部对象-dout)。

#### 宏 Petal_VSDebugOutputT

定义：  

```C++
#if defined(Petal_Enable_Unicode)
#define Petal_VSDebugOutputT(x) Petal_VSDebugOutputW(L##x)
#else
#define Petal_VSDebugOutputT(x) Petal_VSDebugOutputA(x)
#endif
```

相关宏 [`Petal_Enable_Unicode`](./Preprocessor.md#petal_enable_unicode)、[`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)、[`Petal_VSDebugOutputW`](#宏-petal_vsdebugoutputw)。

#### 宏 Petal_VSDebugOutputW

定义：  

```C++
#define Petal_VSDebugOutputW(x) ::Petal::dowt.WriteCStr( x )
```

相关对象 [`dowt`](#命名空间局部对象-dowt)。

#### 宏 Petal_VSDebugOutput_S

定义：  

```C++
#if defined(UNICODE) || defined(_UNICODE)
#define Petal_VSDebugOutput_S(x) Petal_VSDebugOutputW(x)
#else
#define Petal_VSDebugOutput_S(x) Petal_VSDebugOutputA(x)
#endif
```

相关宏 [`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)、[`Petal_VSDebugOutputW`](#宏-petal_vsdebugoutputw)。

#### 宏 Petal_VSDebugOutput_ST

定义：  

```C++
#if defined(UNICODE) || defined(_UNICODE)
#define Petal_VSDebugOutput_ST(x) Petal_VSDebugOutputW(L##x)
#else
#define Petal_VSDebugOutput_ST(x) Petal_VSDebugOutputA(x)
#endif
```

相关宏 [`Petal_VSDebugOutputA`](#宏-petal_vsdebugoutputa)、[`Petal_VSDebugOutputW`](#宏-petal_vsdebugoutputw)。

### 命名空间 Petal

#### 命名空间局部对象 dout

持有类型 [`VSDebugOutputA`](#类-vsdebugoutputa)。用于向 VS 输出窗口输出 `char` 相关的字符串。  

该对象是 `constinit` 的。  

#### 命名空间局部对象 dowt

持有类型 [`VSDebugOutputW`](#类-vsdebugoutputw)。用于向 VS 输出窗口输出 `wchar_t` 相关的字符串。  

该对象是 `constinit` 的。  

### 命名空间 Petal::Abstract

#### 类模板 VSDebugOutput

用于定义接口。模板参数为 `char` 和 `wchar_t` 的实例作为 [`VSDebugOutputA`](#类-vsdebugoutputa) 和 [`VSDebugOutputW`](#类-vsdebugoutputw) 的基类。  

模板参数 `CharT`，`Traits = ::std::char_traits<CharT>`。  

派生自 [`Output<CharT, Traits>`](./Petal~Output.md#模板类-output)。  

##### VSDebugOutput 成员类型别名

* `CharType` 模板参数 `CharT` 的别名；  
* `TraitsType` 模板参数 `Traits` 的别名；  
* `CStringType = Petal::ptrc<CharType>` 接口 `WriteCStr` 的参数类型。  

##### VSDebugOutput 成员函数

###### VSDebugOutput 成员函数 Write

纯虚函数，待派生类实现，该纯虚函数定义于 [`Output`](./Petal~Output.md#output-成员函数-write)，语义为输出字符串参数。  

参数：

* [`str: BasicStringView<CharType, TraitsType>`](./Petal~String.md#类型别名模板-basicstringview) 待输出字符串。  
* 返回值：无。  

###### VSDebugOutput 成员函数 WriteCStr

纯虚函数，待派生类实现，语义为输出 C 风格字符串参数。  

该函数是 `noexcept` 的。  

参数：

* [`c_str: CStringType`](#vsdebugoutput-成员类型别名) 待输出字符串。  
* 返回值：无。  

参数必须是 C 风格字符串(保证 null-terminated)。  

###### VSDebugOutput 成员函数 LnMode

获取换行符，在 Windows 平台，它是 CRLF。  

该函数是 `noexcept` 的。  

是 [`Output` 定义的接口](./Petal~Output.md#output-成员函数-lnmode)的实现。  

该函数无参数，返回值为 [`LineBreakMode`](./Petal~Output.md#枚举类-linebreakmode) 类型。  

### 命名空间 Petal::Debug

#### 函数 print

使用 C++20 `format` 库 和 [`dout`](#命名空间局部对象-dout)、[`dowt`](#命名空间局部对象-dowt) 进行格式化输出，有对 `char` 和 `wchar_t` 的重载。该函数实例化自可变参数模板 `print`。  

参数：  

* `fmt: const std::basic_format_string<CharT, std::type_identity_t<Args>...>` 格式串参数，需要编译时常量；  
* `args: Args&&...` 待格式化输出的对象，需要支持 `formatter`；  
* 返回值：无。  

示例参见[概述](#print-和-println-函数)。  

#### 函数 println

使用 C++20 `format` 库 和 [`dout`](#命名空间局部对象-dout)、[`dowt`](#命名空间局部对象-dowt) 进行格式化行输出，有对 `char` 和 `wchar_t` 的重载。该函数实例化自可变参数模板 `println`。  

参数：  

重载一，函数模板  

* `fmt: const std::basic_format_string<CharT, std::type_identity_t<Args>...>` 格式串参数，需要编译时常量；  
* `args: Args&&...` 待格式化输出的对象，需要支持 `formatter`；  
* 返回值：无。  

重载二，`void println()`  

* 返回值：无。  

示例参见[概述](#print-和-println-函数)。  

#### 类 VSDebugOutputA

使用 `OutputDebugStringA` 实现的 [`VSDebugOutput`](#类模板-vsdebugoutput) 的接口，派生自 [`VSDebugOutput<char>`](#类模板-vsdebugoutput)。  

#### 类 VSDebugOutputW

使用 `OutputDebugStringW` 实现的 [`VSDebugOutput`](#类模板-vsdebugoutput) 的接口，派生自 [`VSDebugOutput<wchar_t>`](#类模板-vsdebugoutput)。  

### 命名空间 Petal::Debug::V

#### 函数 print (namespace Petal::Debug::V)

使用 C++20 `format` 库 和 [`dout`](#命名空间局部对象-dout)、[`dowt`](#命名空间局部对象-dowt) 进行格式化输出，有对 `char` 和 `wchar_t` 的重载。该函数实例化自可变参数模板 `print`。  

参数：  

* `fmt: std::basic_string_view<CharT>` 格式串参数；  
* `args: Args&&...` 待格式化输出的对象，需要支持 `formatter`；  
* 返回值：无。  

示例参见[概述](#print-和-println-函数)。  

#### 函数 println (namespace Petal::Debug::V)

使用 C++20 `format` 库 和 [`dout`](#命名空间局部对象-dout)、[`dowt`](#命名空间局部对象-dowt) 进行格式化行输出，有对 `char` 和 `wchar_t` 的重载。该函数重载可以实例化自可变参数模板 `println`。  

参数：  

重载一，函数模板  

* `fmt: std::basic_string_view<CharT>` 格式串参数；  
* `args: Args&&...` 待格式化输出的对象，需要支持 `formatter`；  
* 返回值：无。  

重载二，`void println()`  

* 返回值：无。  

示例参见[概述](#print-和-println-函数)。  
