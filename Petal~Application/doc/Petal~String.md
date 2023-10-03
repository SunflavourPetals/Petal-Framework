# Petal~String

## 概述

命名空间 Petal

此标头定义了字符类型和字符串类型相关的别名。

 * using Char = achar;
 * using WChar = wchar;
 * using CharU8 = u8char;
 * using CharU16 = u16char;
 * using CharU32 = u32char;
 * using String = typename ::std::basic_string&lt;Char>;
 * using WString = typename ::std::basic_string&lt;WChar>;
 * using StringU8 = typename ::std::basic_string&lt;CharU8>;
 * using StringU16 = typename ::std::basic_string&lt;CharU16>;
 * using StringU32 = typename ::std::basic_string&lt;CharU32>;

achar、wchar、u8char等类型别名详见标头[Petal~BasicTypes.h](Petal~BasicTypes.md)

此标头提供了函数模板`StringToCStyleString`用于制作一个兼容C风格字符串的C++字符串(在串结尾处前不存在空字符)。

[示例](#ref_Petal_TemplateFunction_StringToCStyleString)。

### T系列字符(串)类型

根据宏`Petal_Enable_Unicode`
TChar 将被定义为 defined(Petal_Enable_Unicode) ? WChar : Char;
TString 将被定义为 defined(Petal_Enable_Unicode) ? WString : String;

TChar 和 TString 类似于WIN32中的约定。本框架对WIN32中区分A/W版本的API的使用根据宏`Petal_Enable_Unicode`确定，不受WIN32中宏`UNICODE`及受其影响的宏、别名等如`CreateWindowEx`、`WNDCLASSEX`影响。当定义了宏`Petal_Enable_Unicode`时，统一使用W版本，否则统一使用A版本。TChar 和 TString 随宏`Petal_Enable_Unicode`被定义为不同的字符(串)类型的别名，是与WIN32沟通的桥梁。  
使用宏`Petal_TStr(quote)`将字符串字面量在预处理阶段制作成相应类型的字符串字面量。

### Dbg系列字符(串)类型

根据宏`Petal_Enable_ForceDbgANSI`和宏`Petal_Enable_Unicode`
DbgChar 将被定义为 defined(Petal_Enable_ForceDbgANSI) ? Char : TChar;
DbgString 将被定义为 defined(Petal_Enable_ForceDbgANSI) ? String : TString;

用于指示框架内调试输出使用A/W版本输出函数，详见[Petal~VSDebugOutput.h](Petal~VSDebugOutput.md)。

### Excep系列字符类型

根据宏`Petal_Enable_ForceExcepDescANSI`和宏`Petal_Enable_Unicode`
ExcepChar 将被定义为 defined(Petal_Enable_ForceDbgANSI) ? Char : TChar;

用于指示框架内异常使用A/W版本异常描述字符串，详见[Petal~Exception.h](Petal~Exception.md)。

## 参考

### 全局命名空间

#### 宏 Petal_TStr

用于将 char 字符串字面量`"quote"` 制作为可隐式转换为`ptrc<TChar>`类型的字符串字面量。

#### 宏 Petal_DbgStr

用于将 char 字符串字面量`"quote"` 制作为可隐式转换为`ptrc<DbgChar>`类型的字符串字面量。

#### 宏 Petal_ExcepStr

用于将 char 字符串字面量`"quote"` 制作为可隐式转换为`ptrc<ExcepChar>`类型的字符串字面量。

### 命名空间 Petal

#### 函数模板 StringToCStyleString {#ref_Petal_TemplateFunction_StringToCStyleString}

返回值 ::std::basic_string&lt;CharT, Traits, Alloc> // CharT Traits Alloc 为模板参数

用于制作一个兼容C风格字符串的C++字符串(在串结尾处前不存在空字符)。

示例：
```cpp
#include "Petal~VSDebugOutput.h"
#include "Petal~String.h"
void Test()
{
    Petal::String str;
    str.push_back('a');
    str.push_back('\0');
    str.push_back('b');
    Petal::dout.OutputCStr(Petal::StringToCStyleString(str).c_str()); // 需要C风格字符串！
}
```

结果输出`ab`，如果直接使用
```cpp
Petal::dout.OutputCStr(str.c_str());
```
输出为`a`。

### 命名空间 Petal::EnumChar

列举了部分常用控制字符，详见源文件定义。

### 命名空间 Petal::Concept

	template <typename Ty>
		constexpr bool IsCharType{ ::std::is_same_v<Ty, Char> || ::std::is_same_v<Ty, WChar> || ::std::is_same_v<Ty, CharU8> || ::std::is_same_v<Ty, CharU16> || ::std::is_same_v<Ty, CharU32> };

	template <typename Ty>
		constexpr bool IsStringType{ ::std::is_same_v<Ty, ::std::basic_string<typename Ty::value_type, typename Ty::traits_type, typename Ty::allocator_type>> && IsCharType<typename Ty::value_type> };

	template <typename Ty>
		concept CharType = IsCharType<Ty>;

	template <typename Ty>
		concept StringType = IsStringType<Ty>;

#### 命名空间模板变量 IsCharType

bool 类型，值为 Ty is char series type。

`char` `wchar_t` `char8_t` `char16_t` `char32_t`

#### 命名空间模板变量 IsStringType

bool 类型，值为 Ty is string series type。

`std::basic_string<CharType, Traits, Alloc>`

#### 概念 CharType

Char 系列类型

`char` `wchar_t` `char8_t` `char16_t` `char32_t`

#### 概念 StringType

String 系列类型

`std::basic_string<CharType, Traits, Alloc>`
