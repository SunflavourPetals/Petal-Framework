# Petal~Output

## 概述

命名空间 Petal  

本框架封装了用于 Visual Studio Debug 输出的方式(`OutputDebugStringA/W`)和使用 C++ 标准库的文件流进行 log 输出的方式(这个 log 功能并不是本框架的核心，它只提供了很基础的功能)。  
本头文件中的 `Output` 是它们的基类。  

## 参考

### 宏

#### 宏 Petal_Header_Output

指示头文件引入情况。  

### 命名空间 Petal

#### 函数模板 GetLn

通过 `LineBreakMode mode` 参数获取换行的格式，Windows 平台上通常是 `CRLF`。  

模板参数 `typename CharT`  
形参列表 `LineBreakMode mode`  
返回值类型 `BasicCStringRef<CharT>`  
返回值内容为表示换行的字符串，如 `CRLF`、`LF`、`CR`。  

#### 枚举类 LineBreakMode

用于枚举换行格式，Log 功能的 `Write` 方法是输出二进制，并不会对字符串做处理，包括换行，在 Windows 平台，它是 CRLF，所以有些情况我们需要自己控制换行的形式。  

这个枚举类是 [`Output`](#模板类-output) 的 [`LnMode`](#output-成员函数-lnmode) 方法的返回值的类型。  

```C++
enum class LineBreakMode
{
    CRLF = 1,
    CR = 2,
    LF = 3,
    Default = CRLF,
};
```

#### 函数模板 ln

对 `Output` 对象输出换行，换行格式取决于对象实参的LnMode方法的返回值。  

模板参数 `typename CharT, typename Traits = ::std::char_traits<CharT>`  
形参列表 `LineBreakMode mode`  
返回值类型 `void`  

模板参数的作用参见 [模板类 `Output`](#模板类-output)。  

```C++
using namespace Petal;
ln(dout);
dout << ln;
```

### 命名空间 Petal::Abstract

#### 模板类 Output

作为输出对象的抽象类。  
使用成员函数 `Write` 进行输出；  
使用成员函数 `LnMode` 获得期望的换行格式。  

使用流输出运算符 `<<` 也可以进行输出，但是不像标准库那样提供对相应字符串视图类型之外的重载，推荐使用 `fmt` 库或C++20 的 `format` 库进行格式化，`sstream` 中的内容也可以用于构建要输出的内容，请参阅它们的文档。  

使用例：

```C++
void foo()
{
    using namespace Petal;
    // dout 是 Output 的派生类的对象，声明于 Petal~VSDebugOutput.h
    dout << "Hello world" << ln;
    dout << std::format("today is {}-{}-{}", 5， 6， 2024) << ln;
    // 我们更推荐使用 Petal::Debug 名称空间内的 print 和 println 函数来间接使用 dout
    // 参见 Petal~VSDebugOutput 的文档
}
```

模板参数 `typename CharT, typename Traits = ::std::char_traits<CharT>`。  
`CharT` 为目标输出字符串所使用的字符类型；  
`Traits` 为目标输出字符串所使用的字符类型的特性，默认为 `std::char_traits<CharT>`。  

##### Output 成员类型

###### Output 成员类型 CharType

模板参数 `CharT` 的别名。  

###### Output 成员类型 TraitsType

模板参数 `Traits` 的别名。  

##### Output 成员函数

###### Output 成员函数 Write

作为输出的接口的纯虚函数。  

###### Output 成员函数 LnMode

作为获取换行格式的纯虚函数。  

##### Output 相关非成员函数

重载的 “流输出运算符”，流输出运算符这个名称取自 C++ 标准库中流的 `operator<<`，我们并不把 `Output` 相关的内容当作流。  

实现如下：  

```C++
friend Output& operator<<(Output& out, typename BasicStringView<CharType, TraitsType> str)
{
    out.Write(str);
    return out;
}
friend Output& operator<<(Output& out, const fptr<void, Output&> pfn)
{
    pfn(out);
    return out;
}
```

#### 类型别名 OutputA

`Output<Char>` 的类型别名。  

```C++
using IOutputA = Output<Char>;
```

#### 类型别名 OutputU16

`Output<U16Char>` 的类型别名。  

```C++
using IOutputU16 = Output<U16Char>;
```

#### 类型别名 OutputU32

`Output<U32Char>` 的类型别名。  

```C++
using IOutputU32 = Output<U32Char>;
```

#### 类型别名 OutputU8

`Output<U8Char>` 的类型别名。  

```C++
using IOutputU8 = Output<U8Char>;
```

#### 类型别名 OutputW

`Output<WChar>` 的类型别名。  

```C++
using IOutputW = Output<WChar>;
```
