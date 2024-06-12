# Petal~String

## 概述

命名空间 Petal  

### 别名

此标头定义了字符类型和字符串类型相关的别名/别名模板。  

* `using Char = achar;`
* `using WChar = wchar;`
* `using U8Char = u8char;`
* `using U16Char = u16char;`
* `using U32Char = u32char;`
* `template <...> using BasicString = ::std::basic_string<...>;`(伪代码)
* `using String = BasicString<Char>;`
* `using WString = BasicString<WChar>;`
* `using U8String = BasicString<U8Char>;`
* `using U16String = BasicString<U16Char>;`
* `using U32String = BasicString<U32Char>;`
* `template <...> using BasicStringView = ::std::basic_string_view<...>;`(伪代码)
* `using StringView = BasicStringView<Char>;`
* `using WStringView = BasicStringView<WChar>;`
* `using U8StringView = BasicStringView<U8Char>;`
* `using U16StringView = BasicStringView<U16Char>;`
* `using U32StringView = BasicStringView<U32Char>;`

achar、wchar、u8char等类型别名详见标头[Petal~BasicTypes.h](Petal~BasicTypes.md)  

### 函数模板 StringToCStyleString 概述

此标头提供了函数模板`StringToCStyleString`用于制作一个兼容C风格字符串的C++字符串(在串结尾处前不存在空字符)。  

[示例](#函数模板-stringtocstylestring)。  

### T系列字符(串)类型

根据宏`Petal_Enable_Unicode`  
`TChar` 将被定义为 `defined(Petal_Enable_Unicode) ? WChar : Char` (伪代码)。  
`TString` 将被定义为 `defined(Petal_Enable_Unicode) ? WString : String` (伪代码)。  

`TChar` 和 `TString` 类似于WIN32中的约定。本框架对WIN32中区分A/W版本的API的使用根据宏`Petal_Enable_Unicode`确定，不受WIN32中宏`UNICODE`及受其影响的宏、别名等如`CreateWindowEx`、`WNDCLASSEX`影响。当定义了宏`Petal_Enable_Unicode`时，统一使用W版本，否则尽可能统一使用A版本。TChar 和 TString 随宏`Petal_Enable_Unicode`被定义为不同的字符(串)类型的别名，是框架与WIN32沟通的桥梁。  
使用宏`Petal_TStr(quote)`将字符串字面量在预处理阶段制作成 `TChar[N]` 类型的字符串字面量。  

有关宏 `Petal_Enable_Unicode` 参见[文档](Preprocessor.md#petal_enable_unicode "文档相应章节")。  

### Dbg系列字符(串)类型

根据宏`Petal_Enable_ForceDbgANSI`和宏`Petal_Enable_Unicode`  
`DbgChar` 将被定义为 `defined(Petal_Enable_ForceDbgANSI) ? Char : TChar` (伪代码)  
`DbgString` 将被定义为 `defined(Petal_Enable_ForceDbgANSI) ? String : TString` (伪代码);  

用于框架内调试输出，详见[Petal~VSDebugOutput.h](Petal~VSDebugOutput.md)。  

### CStringRef 系列类型

此标头提供了模板类 `BasicCStringRef`, 用于需要对 null-terminated 做出保证的部分字符串引用之处。  

以后可能会考虑删除“CStringRef”相关的组件。  

C++ 的 `string_view` 不保证 null-terminated，即不提供 `.c_str` 函数，使得 `string_view` 难以和C风格接口协作，故设置了 `CStringRef` 系列类型。但是只有部分地方它能起点作用，框架也没对它做多少支持，`CStringRef` 更多是在框架内由框架使用。  

## 参考

### 宏

#### 宏 Petal_DbgStr

`Petal_DbgStr("quote")` 将被制作为 `const DbgChar[N]` 类型的字符串字面量。  

#### 宏 Petal_Debug_CStringRefIterator

用于标志是否启用 debug 版本的 `CStringRefIterator`。  
参见 [CStringRefIterator](#类模板-cstringrefiterator)。  

#### 宏 Petal_Header_String

指示头文件引入情况。  

#### 宏 Petal_TStr

`Petal_TStr("quote")` 将被制作为 `const TChar[N]` 类型的字符串字面量。  

### 命名空间 Petal

#### 类模板 BasicCStringRef

`CStringRef` 是比 `string_view` 具有更强约束的“字符串引用”，它必须保证 null-terminated，如果使用起始位置加长度的构造方式，当 `ptr[length]` 不为 NUL 时将抛出异常，不推荐使用这种方式构造它的实例，推荐方式如下：  

* 可以从使用相同类型字符的 `std::basic_string<CharT, ...>` 对象构造 `Petal::BasicCStringRef<CharT>` 对象，不抛出异常；  
* 使用用户自定义字面量构造 `Petal::BasicCStringRef<CharT>` 对象，不抛出异常。  

该类型对象可以隐式转换为 `const CharT*`、`std::basic_string_view<CharT, std::char_traits<CharT>>`。  

提供常用操作如 `size`，`length`，`data`，`c_str`，`at`，`operato[]` 和获取迭代器等操作；  
提供转换为标准库字符串和字符串视图的成员函数模板；  
如果想要使用某些字符串视图的功能，推荐使用 `.view<...>()` 函数得到字符串视图再做操作(提供默认参数)；  
提供比较操作。  
特化 `std::formatter` 和重载输出运算符以支持输出操作。  
提供 `hasher`，但没有特化 `std::hash`，需要使用 `hasher` 时手动填入它，  
如 `std::unordered_map<CStringRef, CStringRef::hasher>`。  

命名风格也尽量倾向标准库。  

本组件尽量模拟标准库的用法，并且不推荐用户使用，这个组件通常只在框架内使用，因此不做过多说明。  

```C++
template <typename CharT>
class BasicCStringRef;
```

##### BasicCStringRef 成员类型

成员类型一览：  

```C++
public:

using Hash = typename CStringRefHash<CharT>;

using value_type = CharT;
using pointer = typename ptr<CharT>;
using const_pointer = typename ptrc<CharT>;
using reference = CharT&;
using const_reference = const CharT&;
using const_iterator = CStringRefIterator<value_type>;
using iterator = const_iterator;
using const_reverse_iterator = typename ::std::reverse_iterator<const_iterator>;
using reverse_iterator = const_reverse_iterator;
using size_type = typename ::std::size_t;
using difference_type = typename ::std::ptrdiff_t;
using hasher = Hash;
```

##### BasicCStringRef 成员函数

成员函数一览：  

```C++
public:

[[nodiscard]] constexpr size_type size() const noexcept;
[[nodiscard]] constexpr size_type length() const noexcept;
[[nodiscard]] constexpr const_pointer data() const noexcept;
[[nodiscard]] constexpr const_pointer c_str() const noexcept;
[[nodiscard]] constexpr bool empty() const noexcept;
[[nodiscard]] constexpr const_iterator begin() const noexcept;
[[nodiscard]] constexpr const_iterator end() const noexcept;
[[nodiscard]] constexpr const_iterator cbegin() const noexcept;
[[nodiscard]] constexpr const_iterator cend() const noexcept;
[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept;
[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept;
[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept;
[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;
[[nodiscard]] constexpr const CharT& at(size_type pos) const;

template <typename Traits = ::std::char_traits<CharT>>
[[nodiscard]] constexpr BasicStringView<CharT, Traits> view() const noexcept;

template <typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
[[nodiscard]] constexpr BasicString<CharT, Traits, Alloc> to_string() const 
    noexcept(noexcept(BasicString<CharT, Traits, Alloc>(::std::declval<BasicCStringRef>().view())));

constexpr const CharT& operator[](size_type pos) const noexcept;
constexpr BasicCStringRef& operator= (const BasicCStringRef str) noexcept;
constexpr BasicCStringRef& operator= (::std::nullptr_t) noexcept;
constexpr operator const CharT*() const noexcept;
constexpr operator BasicStringView<CharT>() const noexcept(noexcept(::std::declval<BasicCStringRef>().view()));

// 构造、析构函数

constexpr BasicCStringRef() = default;

template <typename Traits, typename Alloc>
constexpr BasicCStringRef(const BasicString<CharT, Traits, Alloc>& ref_str)；

constexpr BasicCStringRef(const_pointer str, size_type length)；

constexpr BasicCStringRef(const BasicCStringRef&) = default;

constexpr ~BasicCStringRef() = default;
```

##### BasicCStringRef 相关非成员函数

相关非成员函数一览：  

```C++
template <typename CharT>
[[nodiscard]] constexpr bool operator== (const BasicCStringRef<CharT> lhs, const BasicCStringRef<CharT> rhs)
    noexcept(noexcept(lhs.view() == rhs.view()));

template <typename CharT>
[[nodiscard]] constexpr auto operator<=> (const BasicCStringRef<CharT> lhs, const BasicCStringRef<CharT> rhs)
    noexcept(noexcept(lhs.view() <=> rhs.view()));

template <typename CharT, typename Traits = ::std::char_traits<CharT>>
::std::basic_ostream<CharT, Traits>& operator<<(::std::basic_ostream<CharT, Traits>& out, const BasicCStringRef<CharT>& csr);

template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
[[nodiscard]] inline auto StringToCStyleString(const BasicCStringRef<CharT>& in_str)
    -> BasicString<CharT, Traits, Alloc>;

inline namespace Literals
{
    inline namespace CStringRefLiterals
    {
        [[nodiscard]] inline constexpr CStringRef operator""_csr(const Char * str, ::std::size_t length)；
        [[nodiscard]] inline constexpr WCStringRef operator""_csr(const WChar * str, ::std::size_t length)；
        [[nodiscard]] inline constexpr U8CStringRef operator""_csr(const U8Char * str, ::std::size_t length)；
        [[nodiscard]] inline constexpr U16CStringRef operator""_csr(const U16Char * str, ::std::size_t length)；
        [[nodiscard]] inline constexpr U32CStringRef operator""_csr(const U32Char * str, ::std::size_t length)；
    }
}
```

##### BasicCStringRef 类型别名

类型别名一览：  

```C++
using CStringRef = BasicCStringRef<Char>;
using WCStringRef = BasicCStringRef<WChar>;
using U8CStringRef = BasicCStringRef<U8Char>;
using U16CStringRef = BasicCStringRef<U16Char>;
using U32CStringRef = BasicCStringRef<U32Char>;
```

#### 类型别名模板 BasicString

标准库 `basic_string` 的别名。  

```C++
template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
using BasicString = ::std::basic_string<CharT, Traits, Alloc>;
```

#### 类型别名模板 BasicStringView

标准库 `basic_string_view` 的别名。  

```C++
template <typename CharT, typename Traits = ::std::char_traits<CharT>>
using BasicStringView = ::std::basic_string_view<CharT, Traits>;
```

#### 类型别名 Char

`using Char = achar;`

#### 类型别名 CStringRef

`using CStringRef = BasicCStringRef<Char>;`

#### 类模板 CStringRefHash

供 `BasicCStringRef<...>` 使用的 `hasher`。  

```C++
template <typename CharT>
class CStringRefHash;
```

提供 `operator()`。  

示例：  

```C++
using namespace Petal;
auto csr = "string"_csr;
using csr_type = decltype(csr);
csr_type::hasher hasher{}; // CStringRefHash<csr_type::value_type>
auto hash_val = hasher(csr);
```

#### 类模板 CStringRefIterator

为 `CStringRef` 系列类型提供的迭代器类型的模板，具有和标准库字符串视图相似的属性。  
本框架使用 msvc，因此此处提供适用 msvc 的 Debug 版本并模仿其行为。  

可以通过预处理器宏定义控制是否启用 debug 版本的行为，否则根据 `_ITERATOR_DEBUG_LEVEL` 自动选择是否使用 Debug 版本的代码(其中使用了宏`_STL_VERIFY`)，参考[预处理器文档的内容`Petal_Enable_DebugCStringRefIterator`](Preprocessor.md#Petal_Enable_DebugCStringRefIterator "预处理器文档的相应章节")。  

```C++
template <class CharT>
class CStringRefIterator;
```

本组件尽量模拟标准库的用法，并且不推荐用户使用，这个组件通常只在框架内使用，因此不做过多说明，未来可能考虑删除有关“CStringRef”组件相关的内容。  

```C++
public:
#ifdef __cpp_lib_concepts
    using iterator_concept = typename ::std::contiguous_iterator_tag;
#endif
    using iterator_category = typename ::std::random_access_iterator_tag;
    using value_type = CharT;
    using difference_type = typename ::std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    constexpr CStringRefIterator() noexcept = default;
    constexpr CStringRefIterator(const CStringRefIterator&) noexcept = default;
    constexpr CStringRefIterator& operator= (const CStringRefIterator&)noexcept = default;
    constexpr ~CStringRefIterator() noexcept = default;

private:
    friend BasicCStringRef<value_type>;

#ifdef Petal_Debug_CStringRefIterator
    constexpr CStringRefIterator(pointer ref_data, tsize ref_size, tsize ref_off) noexcept;
#else
    constexpr explicit CStringRefIterator(pointer elem_ptr) noexcept;
#endif

public:
    [[nodiscard]] constexpr reference operator*() const noexcept;
    [[nodiscard]] constexpr pointer operator->() const noexcept;
    constexpr CStringRefIterator& operator++() noexcept;
    constexpr CStringRefIterator operator++(int) noexcept;
    constexpr CStringRefIterator& operator--() noexcept;
    constexpr CStringRefIterator operator--(int) noexcept;
    constexpr CStringRefIterator& operator+=(difference_type off) noexcept;
    [[nodiscard]] friend constexpr CStringRefIterator operator+
        (CStringRefIterator left, difference_type off) noexcept;
    [[nodiscard]] friend constexpr CStringRefIterator operator+
        (difference_type off, CStringRefIterator right) noexcept;
    constexpr CStringRefIterator& operator-=(difference_type off) noexcept;
    [[nodiscard]] constexpr CStringRefIterator operator-(difference_type off) const noexcept;
    [[nodiscard]] constexpr difference_type operator-(const CStringRefIterator& right) const noexcept;
    [[nodiscard]] constexpr reference operator[](difference_type off) const noexcept;
    [[nodiscard]] friend constexpr bool operator==
        (const CStringRefIterator& left, const CStringRefIterator& right) noexcept;
    [[nodiscard]] friend constexpr ::std::strong_ordering operator<=>
        (const CStringRefIterator& left, const CStringRefIterator& right) noexcept;
```

#### 类型别名 DbgChar

```C++
#if defined(Petal_Enable_ForceDbgANSI)
    using DbgChar = Char;
#else
    using DbgChar = WChar;
#endif
```

#### 类型别名 DbgCStringRef

```C++
#if defined(Petal_Enable_ForceDbgANSI)
    using DbgCStringRef = CStringRef;
#else
    using DbgCStringRef = WCStringRef;
#endif
```

#### 类型别名 DbgString

```C++
#if defined(Petal_Enable_ForceDbgANSI)
    using DbgString = String;
#else
    using DbgString = WString;
#endif
```

#### 类型别名 DbgStringView

```C++
#if defined(Petal_Enable_ForceDbgANSI)
    using DbgStringView = StringView;
#else
    using DbgStringView = WStringView;
#endif
```

#### 类型别名 String

`using String = BasicString<Char>;`

#### 函数模板 StringToCStyleString

返回值 `::std::basic_string<CharT, Traits, Alloc>` CharT、Traits、Alloc 为模板参数。  

用于制作一个兼容C风格字符串的C++字符串(在串结尾处前不存在空字符)。  

针对参数类型的重载：  

* `::std::basic_string<CharT, Traits, Alloc>`  
* `::std::basic_string_view<CharT, Traits>`  
* `::Petal::CStringRef<CharT>`  
* `InCharT[N]`(InCharT 可以是有 const 限定的类型，但不能是有 volatile 限定的类型)  

示例：  

```cpp
#include "Petal~VSDebugOutput.h"
#include "Petal~String.h"
void Test()
{
    {
        using namespace Petal;
        auto result = StringToCStyleString("const char[15]");
        char char_arr[]{ "char[8]" };
        result = StringToCStyleString(char_arr);
        result = StringToCStyleString("string_view"_sv);
        result = StringToCStyleString("string"_s);
        result = StringToCStyleString("CStringRef"_csr);
    }
    Petal::String str;
    str.push_back('a');
    str.push_back('\0');
    str.push_back('b');
    Petal::dout.OutputCStr(Petal::StringToCStyleString(str).c_str()); // 需要C风格字符串！
}
```

结果输出 `ab`，如果直接使用

```cpp
Petal::dout.OutputCStr(str.c_str());
```

输出为 `a`。

#### 类型别名 StringView

`using StringView = BasicStringView<Char>;`

#### 类型别名 TChar

```C++
#if defined(Petal_Enable_Unicode)
    using TChar = WChar;
#else
    using TChar = Char;
#endif
```

有关宏 `Petal_Enable_Unicode` 参见[文档](Preprocessor.md#petal_enable_unicode "文档相应章节")。  

#### 类型别名 TCStringRef

```C++
#if defined(Petal_Enable_Unicode)
    using TCStringRef = WCStringRef;
#else
    using TCStringRef = CStringRef;
#endif
```

有关宏 `Petal_Enable_Unicode` 参见[文档](Preprocessor.md#petal_enable_unicode "文档相应章节")。  

#### 类型别名 TString

```C++
#if defined(Petal_Enable_Unicode)
    using TString = WString;
#else
    using TString = String;
#endif
```

有关宏 `Petal_Enable_Unicode` 参见[文档](Preprocessor.md#petal_enable_unicode "文档相应章节")。  

#### 类型别名 TStringView

```C++
#if defined(Petal_Enable_Unicode)
    using TStringView = WStringView;
#else
    using TStringView = StringView;
#endif
```

有关宏 `Petal_Enable_Unicode` 参见[文档](Preprocessor.md#petal_enable_unicode "文档相应章节")。  

#### 类型别名 U16Char

`using U16Char = u16char;`

#### 类型别名 U16CStringRef

`using U16CStringRef = BasicCStringRef<U16Char>;`

#### 类型别名 U16String

`using U16String = BasicString<U16Char>;`

#### 类型别名 U16StringView

`using U16StringView = BasicStringView<U16Char>;`

#### 类型别名 U32Char

`using U32Char = u32char;`

#### 类型别名 U32CStringRef

`using U32CStringRef = BasicCStringRef<U32Char>;`

#### 类型别名 U32String

`using U32String = BasicString<U32Char>;`

#### 类型别名 U32StringView

`using U32StringView = BasicStringView<U32Char>;`

#### 类型别名 U8Char

`using U8Char = u8char;`

#### 类型别名 U8CStringRef

`using U8CStringRef = BasicCStringRef<U8Char>;`

#### 类型别名 U8String

`using U8String = BasicString<U8Char>;`

#### 类型别名 U8StringView

`using U8StringView = BasicStringView<U8Char>;`

#### 类型别名 WChar

`using WChar = wchar;`

#### 类型别名 WString

`using WString = BasicString<WChar>;`

#### 类型别名 WStringView

`using WStringView = BasicStringView<WChar>;`

#### 类型别名 WCStringRef

`using WCStringRef = BasicCStringRef<WChar>;`

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

### 内联命名空间 Petal::Literals(inline)::CStringRefLiterals(inline)

#### 用户自定义字面量 operator""_csr

从字符串字面量制作相应类型的 `CStringRef` 对象。  

```C++
using namespace Petal;
auto str = "string"_csr;
auto str = u8"u8string"_csr;
auto str = u"u16string"_csr;
auto str = U"u32string"_csr;
auto str = L"wide char string"_csr;
```

### 内联命名空间 Petal::Literals(inline)::StringLiterals(inline)

#### 用户自定义字面量 operator""_s

从字符串字面量制作相应类型的 `String` 对象。  

```C++
using namespace Petal;
auto str = "string"_s;
auto str = u8"u8string"_s;
auto str = u"u16string"_s;
auto str = U"u32string"_s;
auto str = L"wide char string"_s;
```

### 内联命名空间 Petal::Literals(inline)::StringViewLiterals(inline)

#### 用户自定义字面量 operator""_sv

从字符串字面量制作相应类型的 `StringView` 对象。  

```C++
using namespace Petal;
auto str = "string"_sv;
auto str = u8"u8string"_sv;
auto str = u"u16string"_sv;
auto str = U"u32string"_sv;
auto str = L"wide char string"_sv;
```

### 命名空间 Petal::TypeTraits

#### 命名空间局部模板变量 is_tchar

bool 类型，用于判断模板参数是否是 `TChar` 类型(忽视模板参数的cv限定)。  

#### 命名空间局部模板变量 is_dbg_char

bool 类型，用于判断模板参数是否是 `DbgChar` 类型(忽视模板参数的cv限定)。  
