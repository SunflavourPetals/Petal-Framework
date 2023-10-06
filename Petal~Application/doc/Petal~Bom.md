# Petal~Bom

## 概述

命名空间 Petal::BOM  

此标头提供了类型`Bom`用于定义文本文件的BOM，通常用于[Petal::Log系列](Petal~Log.md)对象创建文本文件。

 * no_bom 无BOM，用于utf-8文本文件和ANSI文本文件
 * utf_8 用于utf-8文本文件
 * utf_16_le 用于utf-16-le文本文件
 * utf_16_be 用于utf-16-be文本文件
 * utf_32_le 用于utf-32-le文本文件
 * utf_32_be 用于utf-32-be文本文件

另外此标头提供了函数`RecommendBom`，根据`CharT`的类型推荐BOM，适用于`char`、`wchar_t`、`char8_t`、`char16_t`、`char32_t`等。

## 参考

### 命名空间 Petal::BOM

#### 类 Bom

##### 构造函数

Bom(tsize size, byte b1 = 0, byte b2 = 0, byte b3 = 0, byte b4 = 0) noexcept;

size为BOM占用字节的大小，范围为0~4。

剩余byte类型参数按顺序前`size`个为BOM的各个字节上的值(无符号8位整数)，其余项无意义，默认设置为0。

##### 方法 Data

返回值 Petal::ptrc&lt;Petal::byte>  

得到Bom对象的BOM，为一个Petal::byte类型数组，大小为4，前[Size()](#ref_BOM_Bom_Size)项有意义，应当被写入文件充当BOM，其余项无意义，不应被写入文件。

##### 方法 Size {#ref_BOM_Bom_Size}

返回值 Petal::tsize  

得到Bom对象有效的字节数。

#### 函数模板 RecommendBom

返回值 const Petal::Bom&  

```cpp
template <typename CharT, Petal::boolean force_utf_8_with_bom = false>
[[nodiscard]] constexpr const Bom& RecommendBom() noexcept;
``````

提供`CharT`类型，提供相应的BOM  

 * char：返回`no_bom`
 * wchar_t：根据本机字节序和`wchar_t`的实现，返回`utf_16_le`或`utf_16_be`或`utf_32_le`或`utf_32_be`
 * char8_t：根据参数`force_utf_8_with_bom`的值选择返回`utf_8`和`no_bom`
 * char16_t：根据本机字节序选择返回`utf_16_le`和`utf_16_be`
 * char32_t：根据本机字节序选择返回`utf_32_le`和`utf_32_be`
 * other：返回`no_bom`
