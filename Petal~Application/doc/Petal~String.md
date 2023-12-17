# Petal~String

源代码已更改，这部分内容还未完成修改。  

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
使用宏`Petal_TStr(quote)`将字符串字面量在预处理阶段制作成T系列类型的字符串字面量。  

### Dbg系列字符(串)类型

根据宏`Petal_Enable_ForceDbgANSI`和宏`Petal_Enable_Unicode`  
DbgChar 将被定义为 defined(Petal_Enable_ForceDbgANSI) ? Char : TChar;  
DbgString 将被定义为 defined(Petal_Enable_ForceDbgANSI) ? String : TString;  

用于框架内调试输出，详见[Petal~VSDebugOutput.h](Petal~VSDebugOutput.md)。  

### Excep系列字符类型

根据宏`Petal_Enable_ForceExcepDescANSI`和宏`Petal_Enable_Unicode`  
ExcepChar 将被定义为 defined(Petal_Enable_ForceDbgANSI) ? Char : TChar;  

用于框架内异常使用的异常描述字符串，详见[Petal~Exception.h](Petal~Exception.md)。  

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

列举了部分常用字符和控制字符如空格、空字符、回车符、换行符、水平制表符等，详见源文件内定义。  

```cpp
constexpr Char cr{ '\r' };   // CR : Carriage Return
constexpr Char lf{ '\n' };   // LF : Line Feed
constexpr Char tab{ '\t' };  // HT : Horizontal Tab
constexpr Char vtab{ '\v' }; // VT : Vertical Tab
constexpr Char space{ ' ' }; // Space
constexpr Char null{ '\0' }; // NUL : Null Character
constexpr Char bell{ '\a' }; // BEL : Bell/Alert/Alarm
```

### 命名空间 Petal::Concept

	template <typename Ty>
	concept CharType = TypeTraits::is_char_type<Ty>;

	template <typename Ty>
	concept String = TypeTraits::is_string<Ty>;

	template <typename Ty>
	concept StringOfCharType = TypeTraits::is_string_of_char_type<Ty>;

#### 概念 CharType

符合`Petal::TypeTraits::is_char_type&lt;Ty>`为`true`的类型。  
详见[is_char_type](#ref_Petal__TypeTraits_is__char__type)。  

#### 概念 String

符合`Petal::TypeTraits::is_string&lt;Ty>`为`true`的类型。  
详见[is_string](#ref_Petal__TypeTraits_is__string)。  

#### 概念 StringOfCharType

符合`Petal::TypeTraits::is_string_of_char_type&lt;Ty>`为`true`的类型。  
详见[is_string_of_char_type](#ref_Petal__TypeTraits_is__string__of__char__type)。  

### 命名空间 Petal::TypeTraits

#### 类模板 IsString {#ref_Petal__TypeTraits_IsString}

使用 IsString&lt;Ty>::value 用于判断模板参数是否是标准库模板类 ::std::basic_string&lt;...> 类型(忽视模板参数的cv限定)，  
继承自 ::std::false_type / ::std::true_type  

#### 命名空间局部模板变量 is_char_type {#ref_Petal__TypeTraits_is__char__type}

bool 类型，用于判断模板参数是否是五种字符类型`char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`之一(忽视模板参数的cv限定)。  

#### 命名空间局部模板变量 is_string {#ref_Petal__TypeTraits_is__string}

bool 类型，用于判断模板参数是否是标准库模板类 ::std::basic_string&lt;...> 类型(忽视模板参数的cv限定)。  
见 [IsString&lt;Ty>::value](#ref_Petal__TypeTraits_IsString)。

#### 命名空间局部模板变量 is_string_of_char_type {#ref_Petal__TypeTraits_is__string__of__char__type}

bool 类型，用于判断模板参数是否既是标准库模板类 ::std::basic_string&lt;Ty, ...> 类型(忽视模板参数的cv限定)，其Ty(忽视cv限定)又是五种字符类型`char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`之一。  
见 [IsString&lt;Ty>::value](#ref_Petal__TypeTraits_IsString)。

#### 命名空间局部模板变量 is_tchar

bool 类型，用于判断模板参数是否是`TChar`类型(忽视模板参数的cv限定)。  

#### 命名空间局部模板变量 is_dbg_char

bool 类型，用于判断模板参数是否是`DbgChar`类型(忽视模板参数的cv限定)。  

#### 命名空间局部模板变量 is_excep_char

bool 类型，用于判断模板参数是否是`ExcepChar`类型(忽视模板参数的cv限定)。  
