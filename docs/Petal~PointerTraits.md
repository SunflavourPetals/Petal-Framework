# Petal~PointerTraits

## 概述

命名空间 Petal::TypeTraits | Petal::Concepts  

指针相关类型的特性，如类型谓词 `IsRawPointer` `IsUniquePointer` 等，以及“移除一层某种指针”、“移除所有某种指针”等类型操作功能。  

### 类型谓词

|    类别   |          名称         |                    注释                   |
| :-------: | :-------------------: | :---------------------------------------- |
| 类模板    | `IsRawPointer`        | 成员属性 `value`: 是裸指针                |
| 类模板    | `IsUniquePointer`     | 成员属性 `value`: 是 `unique_ptr<T, D>`   |
| 类模板    | `IsSharedPointer`     | 成员属性 `value`: 是 `shared_ptr<T>`      |
| 类模板    | `IsWeakPointer`       | 成员属性 `value`: 是 `weak_ptr<T>`        |
| 类模板    | `IsSmartPointer`      | 成员属性 `value`: 是智能指针              |
| 类模板    | `IsAnyPointer`        | 成员属性 `value`: 是任意指针              |
| 变量模板  | `is_raw_pointer`      | 值: 是裸指针                              |
| 变量模板  | `is_unique_pointer`   | 值: 是 `unique_ptr<T, D>`                 |
| 变量模板  | `is_shared_pointer`   | 值: 是 `shared_ptr<T>`                    |
| 变量模板  | `is_weak_pointer`     | 值: 是 `weak_ptr<T>`                      |
| 变量模板  | `is_smart_pointer`    | 值: 是智能指针                            |
| 变量模板  | `is_any_pointer`      | 值: 是任意指针                            |
| 概念      | `RawPointer`          | 裸指针                                    |
| 概念      | `UniquePointer`       | `unique_ptr<T, D>`                        |
| 概念      | `SharedPointer`       | `shared_ptr<T>`                           |
| 概念      | `WeakPointer`         | `weak_ptr<T>`                             |
| 概念      | `SmartPointer`        | 智能指针                                  |
| 概念      | `AnyPointer`          | 任意指针                                  |

### 类型操作

|    类别   |              名称             | 注释 |
| :-------: | :---------------------------: | :--- |
| 类模板    | `PointerElement`              | 成员别名 `Type`: 去除一层指针后的类型，若模板参数不是裸指针或智能指针，则成员别名 `Type` 不存在。 |
| 别名模板  | `PointerElementType`          | 去除指针后的类型，若模板参数不是裸指针或智能指针类型，则错误。 |
| 类模板    | `RemoveOneRawPointer`         | 成员别名 `Type`: 如果模板参数是裸指针，为其去除一层裸指针后的类型，否则为模板参数所示类型。 |
| 类模板    | `RemoveOneUniquePointer`      | 成员别名 `Type`: 如果模板参数是 `unique_ptr<T, D>`，为其去除一层该指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveOneSharedPointer`      | 成员别名 `Type`: 如果模板参数是 `shared_ptr<T>`，为其去除一层该指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveOneWeakPointer`        | 成员别名 `Type`: 如果模板参数是 `weak_ptr<T>`，为其去除一层该指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveOneSmartPointer`       | 成员别名 `Type`: 如果模板参数是智能指针，为其去除一层智能指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveOneAnyPointer`         | 成员别名 `Type`: 如果模板参数是裸指针或智能指针，为其去除一层相应指针后的类型，否则为模板参数所示类型。|
| 别名模板  | `RemoveOneRawPointerType`     | 如果模板参数是裸指针，为其去除一层裸指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveOneUniquePointerType`  | 如果模板参数是 `unique_ptr<T, D>`，为其去除一层该指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveOneSharedPointerType`  | 如果模板参数是 `shared_ptr<T>`，为其去除一层该指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveOneWeakPointerType`    | 如果模板参数是 `weak_ptr<T>`，为其去除一层该指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveOneSmartPointerType`   | 如果模板参数是智能指针，为其去除一层智能指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveOneAnyPointerType`     | 如果模板参数是裸指针或智能指针，为其去除一层相应指针后的类型，否则为模板参数所示类型。 |
| 类模板    | `RemoveAllRawPointer`         | 成员别名 `Type`: 如果模板参数是裸指针，为其去除所有顶层裸指针后的类型，否则为模板参数所示类型。 |
| 类模板    | `RemoveAllUniquePointer`      | 成员别名 `Type`: 如果模板参数是 `unique_ptr<T, D>`，为其去除所有顶层该指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveAllSharedPointer`      | 成员别名 `Type`: 如果模板参数是 `shared_ptr<T>`，为其去除所有顶层该指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveAllWeakPointer`        | 成员别名 `Type`: 如果模板参数是 `weak_ptr<T>`，为其去除所有顶层该指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveAllSmartPointer`       | 成员别名 `Type`: 如果模板参数是智能指针，为其去除所有顶层智能指针后的类型，否则为模板参数所示类型。|
| 类模板    | `RemoveAllAnyPointer`         | 成员别名 `Type`: 如果模板参数是裸指针或智能指针，为其去除所有顶层裸指针和智能指针后的类型，否则为模板参数所示类型。|
| 别名模板  | `RemoveAllRawPointerType`     | 如果模板参数是裸指针，为其去除所有顶层裸指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveAllUniquePointerType`  | 如果模板参数是 `unique_ptr<T, D>`，为其去除所有顶层该指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveAllSharedPointerType`  | 如果模板参数是 `shared_ptr<T>`，为其去除所有顶层该指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveAllWeakPointerType`    | 如果模板参数是 `weak_ptr<T>`，为其去除所有顶层该指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveAllSmartPointerType`   | 如果模板参数是智能指针，为其去除所有顶层智能指针后的类型，否则为模板参数所示类型。 |
| 别名模板  | `RemoveAllAnyPointerType`     | 如果模板参数是裸指针或智能指针，为其去除所有顶层裸指针和智能指针后的类型，否则为模板参数所示类型。|

### 使用示例

```C++
#include <type_traits>
#include "Petal~PointerTraits.h"

void foo()
{
    using namespace Petal::Typetraits;
    static_assert(is_any_pointer<int*>); // int* 是裸指针
    static_assert(!is_unique_pointer<std::unique_ptr<int>*>); // std::unique_ptr<int>* 是裸指针而不是 unique_ptr 指针
    static_assert(std::is_same_v<RemoveAllAnyPointer<std::unique_ptr<std::weak_ptr<int*const>>>, int>);
    // std::unique_ptr<std::weak_ptr<int*const>> 去掉最外层的 unique_ptr
    //  -> std::weak_ptr<int*const> 去掉最外层的 weak_ptr
    //  -> int*const 去掉最外层的 *const
    //  -> int 不是裸指针或智能指针，结束，获得类型 int
}

```

### 特化

特化时请务必同时特化相应模板和其辅助模板，否则行为未定义，参考本框架的源码可以推断此时程序的行为。  

示例：  

```C++
struct MyPtr
{
    using element_type = int;
    element_type* data{};
    ~MyPtr() { delete data; }
};

// 特化 IsSmartPointer
template <>
struct Petal::TypeTraits::IsSmartPointer<MyPtr> : std::true_type {};
// 特化 IsSmartPointer 的 辅助模板 is_smart_pointer
template <>
constexpr bool Petal::TypeTraits::is_smart_pointer<MyPtr>{ Petal::TypeTraits::IsSmartPointer<MyPtr>::value };

void foo()
{
    using namespace Petal::TypeTraits;
    static_assert(std::is_same_v<RemoveAllAnyPointerType<MyPtr>, int>);
}
```

### 依赖关系

1. `IsXPointer | is_x_pointer`
2. `XPointer` (概念)
3. `PointerElement | PointerElementType`
4. `RemoveOneXPointer | RemoveOneXPointerType`
5. `RemoveAllXPointer | RemoveAllXPointerType`

`1` 依赖 标准库
`2` 依赖 `1`
`3` 依赖 `2`
`4` 依赖 `3`
`5` 依赖 `4`

## 参考

### 命名空间 Petal::Concepts

#### 概念 AnyPointer

类型是任意指针类型，包括裸指针和 C++ 标准库的智能指针。  

#### 概念 RawPointer

类型是 C++ 的内建指针类型(裸指针)。  

#### 概念 SharedPointer

类型是 C++ 标准库的 `shared_ptr<SomeType>`。  

#### 概念 SmartPointer

类型是 C++ 标准库的智能指针。  

#### 概念 UniquePointer

类型是 C++ 标准库的 `unique_ptr<SomeType, SomeDeleterType>`。  

#### 概念 WeakPointer

类型是 C++ 标准库的 `weak_ptr<SomeType>`。  

### 命名空间 Petal::TypeTraits

#### 类模板 IsAnyPointer

对符合概念 [`AnyPointer`](#概念-anypointer) 的类型，成员属性 `value` 为 `true`，否则为 `false`。  

辅助变量模板 [`is_any_pointer`](#变量模板-is_any_pointer)。  

#### 类模板 IsRawPointer

对符合概念 [`RawPointer`](#概念-rawpointer) 的类型，成员属性 `value` 为 `true`，否则为 `false`。  

辅助变量模板 [`is_raw_pointer`](#变量模板-is_raw_pointer)。  

#### 类模板 IsSharedPointer

对符合概念 [`SharedPointer`](#概念-sharedpointer) 的类型，成员属性 `value` 为 `true`，否则为 `false`。  

辅助变量模板 [`is_shared_pointer`](#变量模板-is_shared_pointer)。  

#### 类模板 IsSmartPointer

对符合概念 [`SmartPointer`](#概念-smartpointer) 的类型，成员属性 `value` 为 `true`，否则为 `false`。  

辅助变量模板 [`is_smart_pointer`](#变量模板-is_smart_pointer)。  

#### 类模板 IsUniquePointer

对符合概念 [`UniquePointer`](#概念-uniquepointer) 的类型，成员属性 `value` 为 `true`，否则为 `false`。  

辅助变量模板 [`is_unique_pointer`](#变量模板-is_unique_pointer)。  

#### 类模板 IsWeakPointer

对符合概念 [`WeakPointer`](#概念-weakpointer) 的类型，成员属性 `value` 为 `true`，否则为 `false`。  

辅助变量模板 [`is_weak_pointer`](#变量模板-is_weak_pointer)。  

#### 变量模板 is_any_pointer

类型谓词 [`IsAnyPointer`](#类模板-isanypointer) 的辅助模板变量。  

#### 变量模板 is_raw_pointer

类型谓词 [`IsRawPointer`](#类模板-israwpointer) 的辅助模板变量。  

#### 变量模板 is_shared_pointer

类型谓词 [`IsSharedPointer`](#类模板-issharedpointer) 的辅助模板变量。  

#### 变量模板 is_smart_pointer

类型谓词 [`IsSmartPointer`](#类模板-issmartpointer) 的辅助模板变量。  

#### 变量模板 is_unique_pointer

类型谓词 [`IsUniquePointer`](#类模板-isuniquepointer) 的辅助模板变量。  

#### 变量模板 is_weak_pointer

类型谓词 [`IsWeakPointer`](#类模板-isweakpointer) 的辅助模板变量。  

#### 类模板 PointerElement

#### 别名模板 PointerElementType

#### 类模板 RemoveAllAnyPointer

#### 别名模板 RemoveAllAnyPointerType

#### 类模板 RemoveAllRawPointer

#### 别名模板 RemoveAllRawPointerType

#### 类模板 RemoveAllSharedPointer

#### 别名模板 RemoveAllSharedPointerType

#### 类模板 RemoveAllSmartPointer

#### 别名模板 RemoveAllSmartPointerType

#### 类模板 RemoveAllUniquePointer

#### 别名模板 RemoveAllUniquePointerType

#### 类模板 RemoveAllWeakPointer

#### 别名模板 RemoveAllWeakPointerType

#### 类模板 RemoveOneAnyPointer

#### 别名模板 RemoveOneAnyPointerType

#### 类模板 RemoveOneRawPointer

#### 别名模板 RemoveOneRawPointerType

#### 类模板 RemoveOneSharedPointer

#### 别名模板 RemoveOneSharedPointerType

#### 类模板 RemoveOneSmartPointer

#### 别名模板 RemoveOneSmartPointerType

#### 类模板 RemoveOneUniquePointer

#### 别名模板 RemoveOneUniquePointerType

#### 类模板 RemoveOneWeakPointer

#### 别名模板 RemoveOneWeakPointerType
