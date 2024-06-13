# Petal~WinTypes

## 概述

定义了大部分本框架常用的 WIN32 所使用的类型的别名，及部分常量。  

由框架内部使用，暂未服务本框架不使用的 WIN32 类型。  

本头文件引入 `<Windows.h>` 但是做了部分特殊处理，在引入前定义宏 `NOMINMAX`、`WIN32_LEAN_AND_MEAN`，并在引入后取消宏定义(若该宏定义是引入前由本框架定义)。  
无论怎样，取消宏 `CreateEvent` 的定义，定义宏 `WIN32CreateEvent` 替代它。  

## 参考

### 宏

#### 宏 Petal_Header_WinTypes

指示头文件引入情况。  

### 命名空间 Petal

#### 类型别名 Win32CreateStruct

```cpp
#ifdef Petal_Enable_Unicode
    using Win32CreateStruct = typename ::CREATESTRUCTW;
#else
    using Win32CreateStruct = typename ::CREATESTRUCTA;
#endif
```

只受宏 `Petal_Enable_Unicode` 影响。  

#### 类型别名 Win32Message

`using Win32Message = typename ::MSG;`

#### 类型别名 Win32Rect

`using Win32Rect = typename ::RECT;`

#### 类型别名 Win32WindowClass

```cpp
#ifdef Petal_Enable_Unicode
    using Win32WindowClass = typename ::WNDCLASSEXW;
#else
    using Win32WindowClass = typename ::WNDCLASSEXA;
#endif
```

只受宏 `Petal_Enable_Unicode` 影响。  

#### constexpr 命名空间局部变量 win32_false

`inline constexpr win32bool win32_false{ FALSE };`  

代替 WIN32 的 `FALSE` 宏。  

#### constexpr 命名空间局部变量 win32_no_error

`inline constexpr win32error win32_no_error{ ERROR_SUCCESS };`

代替 WIN32 的 `ERROR_SUCCESS` 宏。  

#### constexpr 命名空间局部变量 win32_true

`inline constexpr win32bool win32_true{ TRUE };`  

代替 WIN32 的 `TRUE` 宏。  

#### 类型别名 win32atom

`using win32atom = typename ::ATOM;`

#### 类型别名 win32bool

`using win32bool = typename ::BOOL;`

#### 类型别名 win32byte

`using win32byte = typename ::BYTE;`

#### 类型别名 win32ctstr

```cpp
#ifdef Petal_Enable_Unicode
    using win32ctstr = typename ::LPCWSTR;
#else
    using win32ctstr = typename ::LPCSTR;
#endif
```

不同于 WIN32 中的 `LPCTSTR`，`win32ctstr` 只受宏 `Petal_Enable_Unicode` 影响。  

#### 类型别名 win32dword

`using win32dword = typename ::DWORD;`

#### 类型别名 win32error

`using win32error = decltype(::GetLastError());`

#### 类型别名 win32handle

`using win32handle = typename ::HANDLE;`

#### 类型别名 win32hbrush

`using win32hbrush = typename ::HBRUSH;`

#### 类型别名 win32hcursor

`using win32hcursor = typename ::HCURSOR;`

#### 类型别名 win32hicon

`using win32hicon = typename ::HICON;`

#### 类型别名 win32hins

`using win32hins = typename ::HINSTANCE;`

#### 类型别名 win32hmenu

`using win32hmenu = typename ::HMENU;`

#### 类型别名 win32hwnd

`using win32hwnd = typename ::HWND;`

#### 类型别名 win32int

`using win32int = typename ::INT;`

#### 类型别名 win32long

`using win32long = typename ::LONG;`

#### 类型别名 win32lprm

`using win32lprm = typename ::LPARAM;`

#### 类型别名 win32lptr

`using win32lptr = typename ::LONG_PTR;`

#### 类型别名 win32lres

`using win32lres = typename ::LRESULT;`

#### 类型别名 win32msg

`using win32msg = typename ::UINT;`

在消息循环中，`message` 使用 `UINT` 类型，与结构体 `MSG` 不同。  

#### 类型别名 win32short

`using win32short = typename ::SHORT;`

#### 类型别名 win32tchar

```cpp
#ifdef Petal_Enable_Unicode
    using win32tchar = typename ::CHAR;
#else
    using win32tchar = typename ::WCHAR;
#endif
```

不同于 WIN32 中的 `TCHAR`，`win32tchar` 只受宏 `Petal_Enable_Unicode` 影响。  

#### 类型别名 win32tstr

```cpp
#ifdef Petal_Enable_Unicode
    using win32tstr = typename ::LPWSTR;
#else
    using win32tstr = typename ::LPSTR;
#endif
```

不同于 WIN32 中的 `LPTSTR`，`win32tstr` 只受宏 `Petal_Enable_Unicode` 影响。  

#### 类型别名 win32uint

`using win32uint = typename ::UINT;`

#### 类型别名 win32ulong

`using win32ulong = typename ::ULONG;`

#### 类型别名 win32ulptr

`using win32ulptr = typename ::ULONG_PTR;`

#### 类型别名 win32ushort

`using win32ushort = typename ::USHORT;`

#### 类型别名 win32wndproc

`using win32wndproc = typename ::WNDPROC;`  

#### 类型别名 win32word

`using win32word = typename ::WORD;`

#### 类型别名 win32wprm

`using win32wprm = typename ::WPARAM;`
