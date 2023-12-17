# Petal~Bom

## 概述

## 参考

#### 宏 Petal_Header_Bom

指示头文件添加状态。  

### 命名空间 Petal::BOM

#### 类 Bom

为 `Petal~Log.h` 服务，创建 Unicode 文本文件时需要写入 Bom。  

不可被继承。  

##### 构造函数

`Bom(tsize size, byte b1 = 0, byte b2 = 0, byte b3 = 0, byte b4 = 0) noexcept;`

size 参数为 bom 大小，即字节数，bn 参数为相应字节上的值。  

```cpp
Bom my_bom{ 3, 0xEFu, 0xBBu, 0xBFu };
// | ***** Data of Bom Object ***** |
// | Data()|     valid    | invalid |
// | index |  00  01  02  |   03    |
// | value |  EF  BB  BF  |   00    |
// | ----- | --- | ------------------
// | Size()|     |
// | value |  3  |
```

##### 公有成员函数 Data

`ptrc<byte> Data() const noexcept;`  

得到指向数据的指针。  

得到指向 Bom 对象的 BOM 数据的指针，为一个 `const Petal::byte` 类型数组，大小为4，前Size()项有意义，应当被写入文件充当BOM，其余项无意义，不应被写入文件。  

##### 公有成员函数 Size

`tsize Size() const noexcept;`  

得到Bom对象有效的字节数。  

#### 常量 no_bom

`const Bom no_bom{ 0, 0x00u, 0x00u, 0x00u, 0x00u };`  

适用于 ANSI 编码和 utf-8 编码的 bom (对于 utf-8 的 bom，请查阅 Windows 上使用 utf-8 的历史)。  

#### 常量 utf_8

`const Bom utf_8{ 3, 0xEFu, 0xBBu, 0xBFu, 0x00u };`  

适用于 utf-8 编码的 bom (请查阅 Windows 上使用 utf-8 的历史)。  

#### 常量 utf_16_be

`const Bom utf_16_be{ 2, 0xFEu, 0xFFu, 0x00u, 0x00u };`

适用于 utf-16 be 编码的 bom。  

#### 常量 utf_16_le

`const Bom utf_16_le{ 2, 0xFFu, 0xFEu, 0x00u, 0x00u };`

适用于 utf-16 le 编码的 bom。  

#### 常量 utf_32_be

`const Bom utf_32_be{ 4, 0x00u, 0x00u, 0xFEu, 0xFFu };`

适用于 utf-32 be 编码的 bom。  

#### 常量 utf_32_le

`const Bom utf_32_le{ 4, 0xFFu, 0xFEu, 0x00u, 0x00u };`

适用于 utf-32 le 编码的 bom。  

#### 函数模板 RecommendBom

```cpp
template <typename CharT, boolean force_utf_8_with_bom = false>
[[nodiscard]] constexpr const Bom& RecommendBom() noexcept;
```

根据模板参数 `CharT` 和非类型模板参数 `force_utf_8_with_bom` 和 `std::endian::native` 提供 Bom。  
对于 `wchar_t`，需要使用 `sizeof` 获取其所占字节大小来推测其可能使用 utf-16 还是 utf-32，在 Win 平台上，它使用 utf-16 le，占两个字节。  

| CharT | std::endian::native | force_utf_8_with_bom | size | Result |
| :---: | :---: | :---: | :---: | :--: |
| char | | | | no_bom |
| wchar_t | std::endian::little | | 2 | utf_16_le |
| wchar_t | std::endian::big | | 2 | utf_16_be |
| wchar_t | std::endian::little | | 4 | utf_32_le |
| wchar_t | std::endian::big | | 4 | utf_32_be |
| char8_t | | true | | utf_8 |
| char8_t | | false | | no_bom |
| char16_t | std::endian::little | | | utf_16_le |
| char16_t | std::endian::big | | | utf_16_be |
| char32_t | std::endian::little | | | utf_32_le |
| char32_t | std::endian::big | | | utf_32_be |
| other | | | | no_bom |
