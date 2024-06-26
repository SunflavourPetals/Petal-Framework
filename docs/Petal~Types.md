# Petal~Types

## 概述

定义了本框架的常用类型。  

## 参考

### 宏

#### 宏 Petal_Header_Types

指示头文件引入情况。  

### 命名空间 Petal

#### 类模板 Position2D

```cpp
template <typename Ty>
struct Position2D
{
    Ty x{};
    Ty y{};
    friend bool operator==(const Position2D&, const Position2D&) noexcept = default;
};
```

提供预置的 `==` 和 `!=` 操作；

#### 类型别名 Position2DF32

`using Position2DF32 = Position2D<f32>;`

#### 类型别名 Position2DI32

`using Position2DI32 = Position2D<i32>;`

#### 类型别名 Position2DU32

`using Position2DU32 = Position2D<u32>;`

#### 类模板 Position3D

```cpp
template <typename Ty>
struct Position3D
{
    Ty x{};
    Ty y{};
    Ty z{};
    friend bool operator==(const Position3D&, const Position3D&) noexcept = default;
};
```

提供预置的 `==` 和 `!=` 操作；

#### 类型别名 Position3DF32

`using Position3DF32 = Position3D<f32>;`

#### 类型别名 Position3DI32

`using Position3DI32 = Position3D<i32>;`

#### 类型别名 Position3DU32

`using Position3DU32 = Position3D<u32>;`

#### 类模板 Rectangle

```cpp
template <typename Ty>
struct Rectangle
{
    Position2D<Ty> left_top;
    Position2D<Ty> right_bottom;
    friend bool operator==(const Rectangle&, const Rectangle&) noexcept = default;
};
```

提供预置的 `==` 和 `!=` 操作；

#### 类型别名 RectangleF32

`using RectangleF32 = Rectangle<f32>;`

#### 类型别名 RectangleI32

`using RectangleI32 = Rectangle<i32>;`

#### 类型别名 RectangleU32

`using RectangleU32 = Rectangle<u32>;`

#### 类模板 Size2D

```cpp
template <typename Ty>
struct Size2D
{
    Ty width{};
    Ty height{};
    friend bool operator==(const Size2D&, const Size2D&) noexcept = default;
};
```

提供预置的 `==` 和 `!=` 操作；

#### 类型别名 Size2DF32

`using Size2DF32 = Size2D<f32>;`

#### 类型别名 Size2DI32

`using Size2DI32 = Size2D<i32>;`

#### 类型别名 Size2DSz

`using Size2DSz = Size2D<tsize>;`

#### 类型别名 Size2DU32

`using Size2DU32 = Size2D<u32>;`
