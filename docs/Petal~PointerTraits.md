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

`is_any_pointer` 被定义为 `is_smart_pointer<Ty> || is_raw_pointer<Ty>`，当特化 `IsAnyPointer` 和 `is_any_pointer` 为其添加新内容时，`PointerElement` 和 `PointerElementType` 也应该对其进行特化，以支持添加的新内容，否则 `RemoveOneXPointer | RemoveOneXPointerType` 和 `RemoveAllXPointer | RemoveAllXPointerType` 将可能错误。  

对 `is_smart_pointer` 特化以添加内容，只要存在成员类型或成员类型别名 `element_type` 就无需特化 `PointerElement` 和 `PointerElementType`，否则不应该特化`IsSmartPointer` 和 `is_smart_pointer`。  

### 依赖关系

1. `IsXPointer | is_x_pointer`
2. `XPointer` (概念)
3. `PointerElement | PointerElementType`
4. `RemoveOneXPointer | RemoveOneXPointerType`
5. `RemoveAllXPointer | RemoveAllXPointerType`

`1` 依赖 标准库  
`2` 依赖 `1`  
`3` 依赖 `2`，符合智能指针概念时依赖其成员类型别名 `element_type`，符合裸指针概念时依赖标准库 `remove_pointer_t`  
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

模板参数 `Ty`。  

当类型满足概念 [`SmartPointer`](#概念-smartpointer) 时：  
成员类型 `Type` 为 `Ty::element_type`。  

当类型满足概念 [`RawPointer`](#概念-rawpointer) 时：  
成员类型 `Type` 为 `std::remove_pointer_t<Ty>`。  

否则成员类型 `Type` 未声明。  

#### 别名模板 PointerElementType

类型操作 [`PointerElement`](#类模板-pointerelement) 的辅助别名模板。  
当模板参数不满足概念 [`SmartPointer`](#概念-smartpointer) 或 [`RawPointer`](#概念-rawpointer) 时，将发生错误。  

#### 类模板 RemoveAllAnyPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 循环去除顶层符合概念 [`AnyPointer`](#概念-anypointer) 类型直到得到的类型不再符合该概念的类型。  

示例：  

```C++
#include "Petal~PointerTraits.h"

template <typename T>
struct MyData
{
    T data{};
};

template <typename T>
using MyDataPtr = MyData<T>*;

void foo2()
{
    static_assert(
        std::is_same_v<
            Petal::TypeTraits::RemoveAllAnyPointer<
                const volatile std::unique_ptr<
                    MyDataPtr<int*>
                >
                *const
                *volatile
            >::Type,
            MyData<int*>
        >
    );
}
```

步骤：  

1. `const volatile std::unique_ptr<MyDataPtr<int*>> *const *volatile` 移除裸指针 `*volatile`
2. `const volatile std::unique_ptr<MyDataPtr<int*>> *const` 移除裸指针 `*const`
3. `const volatile std::unique_ptr<MyDataPtr<int*>>` 移除 `const volatile std::unique_ptr<>`
4. `MyDataPtr<int*>` = `MyData<int*> *` 移除 `*`
5. `MyData<int*>` 不符合概念 [`AnyPointer`](#概念-anypointer)，得到结果

#### 别名模板 RemoveAllAnyPointerType

类型操作 [`RemoveAllAnyPointer`](#类模板-removeallanypointer) 的辅助变量模板。  

#### 类模板 RemoveAllRawPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 循环去除顶层符合概念 [`RawPointer`](#概念-anypointer) 类型直到得到的类型不再符合该概念的类型。  

#### 别名模板 RemoveAllRawPointerType

类型操作 [`RemoveAllRawPointer`](#类模板-removeallrawpointer) 的辅助变量模板。  

#### 类模板 RemoveAllSharedPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 循环去除顶层符合概念 [`SharedPointer`](#概念-sharedpointer) 类型直到得到的类型不再符合该概念的类型。  

#### 别名模板 RemoveAllSharedPointerType

类型操作 [`RemoveAllSharedPointer`](#类模板-removeallsharedpointer) 的辅助变量模板。  

#### 类模板 RemoveAllSmartPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 循环去除顶层符合概念 [`SmartPointer`](#概念-smartpointer) 类型直到得到的类型不再符合该概念的类型。  

#### 别名模板 RemoveAllSmartPointerType

类型操作 [`RemoveAllSmartPointer`](#类模板-removeallsmartpointer) 的辅助变量模板。  

#### 类模板 RemoveAllUniquePointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 循环去除顶层符合概念 [`UniquePointer`](#概念-uniquepointer) 类型直到得到的类型不再符合该概念的类型。  

#### 别名模板 RemoveAllUniquePointerType

类型操作 [`RemoveAllUniquePointer`](#类模板-removealluniquepointer) 的辅助变量模板。  

#### 类模板 RemoveAllWeakPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 循环去除顶层符合概念 [`WeakPointer`](#概念-weakpointer) 类型直到得到的类型不再符合该概念的类型。  

#### 别名模板 RemoveAllWeakPointerType

类型操作 [`RemoveAllWeakPointer`](#类模板-removeallweakpointer) 的辅助变量模板。  

#### 类模板 RemoveOneAnyPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 去除一层符合概念 [`AnyPointer`](#概念-anypointer) 类型的类型。  

#### 别名模板 RemoveOneAnyPointerType

类型操作 [`RemoveOneAnyPointer`](#类模板-removeoneanypointer) 的辅助变量模板。  

#### 类模板 RemoveOneRawPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 去除一层符合概念 [`RawPointer`](#概念-rawpointer) 类型的类型。  

#### 别名模板 RemoveOneRawPointerType

类型操作 [`RemoveOneRawPointer`](#类模板-removeonerawpointer) 的辅助变量模板。  

#### 类模板 RemoveOneSharedPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 去除一层符合概念 [`SharedPointer`](#概念-sharedpointer) 类型的类型。  

#### 别名模板 RemoveOneSharedPointerType

类型操作 [`RemoveOneSharedPointer`](#类模板-removeonesharedpointer) 的辅助变量模板。  

#### 类模板 RemoveOneSmartPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 去除一层符合概念 [`SmartPointer`](#概念-smartpointer) 类型的类型。  

#### 别名模板 RemoveOneSmartPointerType

类型操作 [`RemoveOneSmartPointer`](#类模板-removeonesmartpointer) 的辅助变量模板。  

#### 类模板 RemoveOneUniquePointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 去除一层符合概念 [`UniquePointer`](#概念-uniquepointer) 类型的类型。  

#### 别名模板 RemoveOneUniquePointerType

类型操作 [`RemoveOneUniquePointer`](#类模板-removeoneuniquepointer) 的辅助变量模板。  

#### 类模板 RemoveOneWeakPointer

模板参数 `Ty`。  

成员类型 `Type` 为模板参数 `Ty` 去除一层符合概念 [`WeakPointer`](#概念-weakpointer) 类型的类型。  

#### 别名模板 RemoveOneWeakPointerType

类型操作 [`RemoveOneWeakPointer`](#类模板-removeoneweakpointer) 的辅助变量模板。  
