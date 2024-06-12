# Petal~Bom

## 概述

此标头提供了类型`Bom`用于定义文本文件的BOM，通常用于[Petal::Log系列](Petal~Log.md)对象创建文本文件。

| BOM | 描述 |
| :---: | :---: |
| no_bom | 无BOM，可用于 utf-8 文本文件 |
| utf_8 | 用于 utf-8 文本文件 |
| utf_16_le | 用于 utf-16-le 文本文件 |
| utf_16_be | 用于 utf-16-be 文本文件 |
| utf_32_le | 用于 utf-32-le 文本文件 |
| utf_32_be | 用于 utf-32-be 文本文件 |

另外此标头提供了函数`RecommendBom`，根据`CharT`的类型推荐BOM，适用于`char`、`wchar_t`、`char8_t`、`char16_t`、`char32_t`等。  

## 参考

### 宏

#### 宏 Petal_Header_Bom

指示头文件引入情况。  

### 命名空间 Petal::BOM

#### 类 Bom

为 `Petal~Log.h` 服务，创建 Unicode 文本文件时需要写入 Bom。  

不可被继承。  

##### 构造函数

```C++
Bom(byte b1, byte b2, byte b3, byte b4) noexcept;
Bom(byte b1, byte b2, byte b3) noexcept;
Bom(byte b1, byte b2) noexcept;
Bom(byte b1) noexcept;
Bom() noexcept;
```

`bn` 参数为第 n 字节上的值，成员属性 size 根据所选用的构造函数确定。  

```cpp
// 示例
Bom my_bom{ 0xEFu, 0xBBu, 0xBFu };
// +-----------------------------------+
// |        Data of Bom Object         |
// +-----------------------------------+
// | Data()|  valid   |    invalid     |
// |-------+----------+----------------+
// | index | 00 01 02 | 03 04 05 06 07 |
// | value | EF BB BF | 00 00 00 00 00 |
// +-------+---------------------------+
// | Size()|                           |
// |-------+---------------------------|
// | value |            3              |
// +-----------------------------------+
```

##### 公有成员函数 Data

`ptrc<byte> Data() const noexcept;`  

得到指向数据的指针。  

得到指向 Bom 对象的 BOM 数据的指针，为一个 `const Petal::byte` 类型数组，大小为8，前`Size()` 个元素有意义，应当被写入文件充当BOM，其余项无意义，不应被写入文件。  

##### 公有成员函数 Size

`tsize Size() const noexcept;`  

得到Bom对象有效的字节数。  

#### 命名空间局部常量 no_bom

`inline const Bom no_bom{};`  

适用于 ANSI 编码和 utf-8 编码的 bom 对象，它代表无 BOM (对于 utf-8 的 bom，请查阅 Windows 上使用 utf-8 的历史)。  

#### 命名空间局部常量 utf_8

`inline const Bom utf_8{ 0xEFu, 0xBBu, 0xBFu };`  

适用于 utf-8 编码的 bom 对象 (请查阅 Windows 上使用 utf-8 的历史)。  

#### 命名空间局部常量 utf_16_be

`inline const Bom utf_16_be{ 0xFEu, 0xFFu };`

适用于 utf-16 be 编码的 bom 对象。  

#### 命名空间局部常量 utf_16_le

`inline const Bom utf_16_le{ 0xFFu, 0xFEu };`

适用于 utf-16 le 编码的 bom 对象。  

#### 命名空间局部常量 utf_32_be

`inline const Bom utf_32_be{ 0x00u, 0x00u, 0xFEu, 0xFFu };`

适用于 utf-32 be 编码的 bom 对象。  

#### 命名空间局部常量 utf_32_le

`inline const Bom utf_32_le{ 0xFFu, 0xFEu, 0x00u, 0x00u };`

适用于 utf-32 le 编码的 bom 对象。  

#### 函数模板 RecommendBom

```cpp
template <typename CharT, boolean force_utf_8_with_bom = false>
[[nodiscard]] constexpr const Bom& RecommendBom() noexcept;
```

根据模板参数 `CharT` 和非类型模板参数 `force_utf_8_with_bom` 和 `std::endian::native` 提供 Bom。  
对于 `char`，永远为其提供 `no_bom` 对象；  
对于 `wchar_t`，本框架只用于 Windows 平台，Windows 平台上的 `wchar_t` 使用 utf-16-le，在 Windows 平台，`sizeof(wchar_t)` 为 `2` 并且 `std::endian` 为 `std::little`，C++ 标准并未规定 `wchar_t` 的具体实现，但我们根据平台为其提供对象 `utf_16_le`。  

| CharT | std::endian::native | force_utf_8_with_bom | sizeof(CharT) | result |
|:---:|:---:|:---:|:---:|:---:|
| char | | | | no_bom |
| wchar_t | std::endian::little | | 2 | utf_16_le |
| wchar_t | std::endian::little | | 4 | utf_32_le |
| wchar_t | std::endian::big | | 2 | utf_16_be |
| wchar_t | std::endian::big | | 4 | utf_32_be |
| char8_t | | true | | utf_8 |
| char8_t | | false | | no_bom |
| char16_t | std::endian::little | | | utf_16_le |
| char16_t | std::endian::big | | | utf_16_be |
| char32_t | std::endian::little | | | utf_32_le |
| char32_t | std::endian::big | | | utf_32_be |
| other | | | | no_bom |
